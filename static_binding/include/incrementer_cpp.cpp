#include <starpu.h>
#include  <vector>

#define FPRINTF(ofile, fmt, ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ## __VA_ARGS__); }} while(0)

unsigned niter = 80;
/*-----------------------------------------------------------------------------------------*/
typedef double  ElementType;
class Data;
class Task;
/*-----------------------------------------------------------------------------------------*/
class StarPU{
public:
  int ret;
  StarPU(){
    ret=starpu_init(NULL);    
    STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");
  }
  ~StarPU(){
    starpu_shutdown();
  }
  static void data_created(Data *d);
  static void destroy_data(Data *d);
  static int submit(Task *t);
  static void barrier(){
    starpu_task_wait_for_all();
  }
  
};
/*-----------------------------------------------------------------------------------------*/
class Data{
public:
  int M,N;
  ElementType *mem;
  void *guest;
  Data(int M_,int N_, ElementType e=0.0):M(M_),N(N_){
    mem = new ElementType[M*N];
    for ( int i=0;i<M*N;i++)
      mem[i]=e;
    StarPU::data_created(this);
  }
  ElementType operator[](int i){
    return mem[i];
  }
  ElementType get(int i){
    return mem[i];
  }
  ~Data(){
    StarPU::destroy_data(this);
    delete [] mem;
  }
  void print(){
    FPRINTF(stderr, "array -> %f, %f, %f, %f\n", mem[0],mem[1],mem[2],mem[3]);
  }
};
/*-----------------------------------------------------------------------------------------*/
class SPUData{
public:
  starpu_data_handle_t handle;
  Data *gdata;
  SPUData(Data *x){
    gdata = x;
    int M = gdata->M;
    int N = gdata->N;
    ElementType *mem = gdata->mem;
    starpu_matrix_data_register(&handle, 0, (uintptr_t)mem, N, N, M, sizeof(ElementType));
  }
  void destroy(){
    starpu_data_unregister(handle ) ;
  }
  ~SPUData(){ destroy();  }
};
/*-----------------------------------------------------------------------------------------*/
class SPUTask{
public:
  struct starpu_codelet *clp;
  Task *gtask;
  SPUTask(Task *t):gtask(t){
    clp = new starpu_codelet;
    starpu_codelet_init(clp);
    starpu_codelet &cl = *clp;
    cl.cpu_funcs[0] =SPUTask::run;
    cl.nbuffers = 2;
    cl.modes[0] = STARPU_RW ;
    cl.modes[1] = STARPU_R  ;
    cl.name = "incrementer";
  }
   static void run(void *descr[], void *_args);
};
/*--------------------------------------------------------------------*/
class Task{
public:
  std::vector<Data*>  args;
  SPUTask *stask;
  Task( Data *A,Data *B){
    args.resize(2);
    args[0]= A;
    args[1]= B;
    stask =  new SPUTask(this);
  }
  /*-------------------------------*/
  starpu_task *get_spu_task(){
    starpu_task *spu_task = starpu_task_create();
    spu_task->cl = stask->clp;
    spu_task->cl_arg = (void  *) stask;
    spu_task->cl_arg_size = 100*sizeof(SPUTask );
    
    SPUData *a = static_cast<SPUData *>(args[0]->guest);
    SPUData *b = static_cast<SPUData *>(args[1]->guest);
    spu_task->handles[0] = a->handle;
    spu_task->handles[1] = b->handle;
    return spu_task;
  }
  /*-------------------------------*/
    void run(void *descr[], void *_args){
	ElementType  *A  = (ElementType *)STARPU_VECTOR_GET_PTR(descr[0]);
	ElementType  *B  = (ElementType *)STARPU_VECTOR_GET_PTR(descr[1]);

	A[0] += 1.0f + B[0];
	A[1] += 1.0f + B[1];
  }
  /*-------------------------------*/
  int  check(){
    Data &A= *args[0];
    Data &B= *args[1];
    if ( A[0] != niter * (1+B[0]) ){
      FPRINTF(stderr, "Incorrect result\n");
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }
};
/*----------------------------------------------------------*/
class TaskMinus: public Task {
public:
  TaskMinus(Data * A, Data *B):Task(A,B){}
  void run(void *descr[], void *_args){
    ElementType  *A  = (ElementType *)STARPU_VECTOR_GET_PTR(descr[0]);
    ElementType  *B  = (ElementType *)STARPU_VECTOR_GET_PTR(descr[1]);

    A[1] += 1.0f + B[1];
  }
};
/*----------------------------------------------------------*/
  void SPUTask::run(void *descr[], void *_args){
    Task *t= (Task *)_args;
    if ( t )
      t->run(descr,_args);
    else
      printf("null task *\n");
      
  }

int StarPU::submit(Task *t){
    int ret = starpu_task_submit(t->get_spu_task() );
    if (STARPU_UNLIKELY(ret == -ENODEV))
      {
	FPRINTF(stderr, "No worker may execute this task\n");
	exit(77);
      }
    return ret;
  
  }
void StarPU::data_created(Data *d){
  SPUData *x = new SPUData (d);
  d->guest = static_cast<void *>(x);
}
void StarPU::destroy_data(Data *d){
  SPUData *x  = static_cast<SPUData *>(d->guest );
  x->destroy();
}

/*-----------------------------------------------------------------------------------------*/

int main(int argc, char **argv)
{

	StarPU S;
	
	if (S.ret == -ENODEV)
	  return 77;
	
	Data *A =  new Data (4,1);
	Data *B =  new Data (4,1,100.0);

	Task *t;
	TaskMinus *tt;
	for (unsigned i = 0; i < niter/2; i++)   {
	  t = new Task(A,B);
	  tt = new TaskMinus(A,B);
	  StarPU::submit(t);
	  StarPU::submit(tt);
	}

	StarPU::barrier();

	A->print();	
	int ret =  t->check();

	
	delete A;
	delete B;
	

	return ret;
}
