#include "GData.hpp"
#include "Dispatcher.hpp"
/*=====================================================================*/
GData::GData()
{
    handle = new GHandle;
    memory = nullptr;
    content = nullptr;
    guest = nullptr;
    level =0;
    child_cnt = 0;
}
/*=====================================================================*/
GData::GData(int i,int j,string n):M(i),N(j),name(n) 
{
    handle = new GHandle;
    memory = nullptr;
    content = nullptr;
    child_cnt = 0;
    child_idx=0;
    parent = nullptr;
    children= nullptr;
    partitioner=nullptr;
    guest = nullptr;
    level = 0;

    get_dispatcher()->data_created(this); 
}
/*=====================================================================*/
GData::GData(int i,int j,string n,GData *p, int idx):
  M(i),N(j),child_idx(idx),parent(p),name(n)
{
    handle = new GHandle;
    memory = nullptr;
    content = nullptr;
    child_cnt = 0;
    children= nullptr;
    partitioner=nullptr;
    guest = nullptr;
    if ( p)
      level = p->level+1;
    else
      level = 0;
    get_dispatcher()->data_created(this);
}
/*=====================================================================*/
string GData::get_name()
{
  return name;
}
/*=====================================================================*/
GData::~GData()
{
  LOG_INFO(LOG_MLEVEL,"%s\n",get_name().c_str());
    //dtor
}

/*=====================================================================*/
GData::GData(const GData& other)
{
    //copy ctor
}

/*=====================================================================*/
GData& GData::operator=(const GData& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
/*=====================================================================*/
GData *GData::get_parent()
{
  return parent;
}
/*=====================================================================*/
GPartitioner *GData::get_partition()
{
  return partitioner;
}
/*=====================================================================*/
void GData::set_partition(GPartitioner *P)
{
    partitioner = P;
    assert(P);
    child_cnt = P->y * P->x;
    children = new GData*[child_cnt];
    assert(P->y);
    assert(P->x);
    int m = M/P->y;
    int n = N/P->x;
    for(int i=0; i<child_cnt; i++)
    {
      int px = i/P->y;
      int py = i%P->y;
      ostringstream os;
      os << name 
	 << "_" << setw(2) << setfill('0') << py
	 << "_" << setw(2) << setfill('0') << px;
      string ch_name = os.str();
      //LOG_INFO(LOG_MLEVEL,"Child data :%s of %dx%d at blk(%d,%d) is being created.\n",ch_name.c_str(),m,n,py,px);
      children[i] = new GData(m,n,ch_name,this,i);
    }
    get_dispatcher()->data_partitioned(this);
    GPartitioner *next_p=partitioner->get_next();
    if ( next_p != nullptr)
      for(int i=0; i<child_cnt; i++)
	{
	  //LOG_INFO(LOG_MLEVEL,"Cascade partition to %s with %dx%d\n",children[i]->getName().c_str(),next_p->y,next_p->x);
	  children[i]->set_partition(next_p);
	}
}
/*=====================================================================*/
int GData::get_part_countY()
{
    return partitioner->y;
}
/*=====================================================================*/
int GData::get_part_countX()
{
    return partitioner->x;
}
/*=====================================================================*/
GData &GData::operator ()(int i,int j )
{
    return get_part(i,j);
}
/*=====================================================================*/
GData &GData::get_part(int i , int j )
{
    return *(children[j*partitioner->y+i]);
}
/*=====================================================================*/
GHandle *GData::get_handle()
{
    return handle;
}
/*=====================================================================*/
long GData::get_element_count()
{
    return M*N;
}
/*=====================================================================*/
void GData::set_memory(MemoryItem *m)
{
    memory = m;
  if ( m ) {
    content = m->getAddress();
    assert(content);
  }
}
/*=====================================================================*/
void GData::set_memory(void *m)
{
  content = (byte *)m;
}
/*=====================================================================*/
bool GData::is_memory_allocated()
{
    return content != nullptr;
}
/*=====================================================================*/
int GData::get_rows()
{
    return M;
}
/*=====================================================================*/
int GData::get_cols()
{
    return N;
}
/*=====================================================================*/
byte *GData::get_memory()
{
    return content;
}
/*=====================================================================*/
void GData::fill_with(double v)
{
  double *d = (double*)content;
  if ( d != nullptr){    
    for ( int i=0;i<M*N;i++){
      d[i]=v;
    }
  }
  for ( int c=0;c<child_cnt; c++){
    if ( children[c] != nullptr){
      children[c]->fill_with(v);
    }
  }
}
/*=====================================================================*/
void GData::print()
{
  if ( M > 15 ) return;
  double *d = (double*)content;
  if ( d != nullptr){    
    int py,px,by;
    if ( partitioner ){
      py = M/partitioner->y;
      px = N/partitioner->x;
      by = partitioner->y;
    }
    else {
      py = M;
      px = N;
      by = 1;
    }
    for ( int i=0;i<M;i++){
      for ( int j=0;j<N;j++){
	int ii=i/py;
	int jj=j/px;
	printf("%+02.2lf ",d[jj*py*px*by+ii*py*px+(j%px)*py+i%py]);
      }
      printf("\n");
    }
  }
  printf("%s,child cnt :%d\n",get_name().c_str(),child_cnt);
  for ( int i=0;i<child_cnt;i++){
    if ( children[i] != nullptr)
      children[i]->print();
  }
}
/*=====================================================================*/
void GData::fill_rows_with ( double start , double scale)
{
  double *d = (double*)content;
  if ( d != nullptr){    
    int py = M/partitioner->y;
    int px = N/partitioner->x;
    int by = partitioner->y;
    for ( int i=0;i<M;i++){
      for ( int j=0;j<N;j++){
	int ii=i/py;
	int jj=j/px;
	d[jj*py*px*by+ii*py*px+(j%px)*py+i%py]=start + i*scale;
      }
    }
  }
}
/*=====================================================================*/
void GData ::dump()
{
  if ( M > 5 ) return;
  double *d = (double*)content;
  if ( d != nullptr){    
    for ( int i=0;i<M;i++){
      for ( int j=0;j<N;j++){

	printf("%+02.2lf ",d[j*M+i]);
      }
      printf("\n");
    }
  }
}
/*=====================================================================*/
void GData::fill_moler()
{
  double *d = (double*)content;
  if ( d != nullptr){    
    int py = M/partitioner->y;
    int px = N/partitioner->x;
    int by = partitioner->y;
    for ( int i=0;i<M;i++){
      for ( int j=0;j<N;j++){
	int ii=i/py;
	int jj=j/px;
	d[jj*py*px*by+ii*py*px+(j%px)*py+i%py]=(i==j)?i+1:min(i,j) -1;
      }
    }
  }
}
/*=====================================================================*/
void GData::fill_chol_diag()
{
  double *d = (double*)content;
  if ( d != nullptr){    

    int py = M/partitioner->y;
    int px = N/partitioner->x;
    int by = partitioner->y;
    for ( int i=0;i<M;i++){
      for ( int j=0;j<N;j++){
	int ii=i/py;
	int jj=j/px;
	d[jj*py*px*by+ii*py*px+(j%px)*py+i%py]=(i==j)?1:-1;
      }
    }
  }
}
/*=====================================================================*/
int GData::get_child_index()
{
  return child_idx;
}
/*=====================================================================*/
void GData::getCoordination( int &r, int &c, int &d)
{
  r = c= d = 0;
  assert(parent);
  GPartitioner *p = parent->partitioner;

  if (p!= nullptr){
    if ( p->y ) {
      c = child_idx / p->y;
      r = child_idx % p->y;
      d=0;
    }
  }
}
/*=====================================================================*/
void GData::set_level(int l)
{
  level = l;
}
/*=====================================================================*/
int GData::get_level()
{
  return level;
}
/*=====================================================================*/
void *GData::get_guest(){return guest;}
void GData::set_guest(void *p){guest=p;}
/*=====================================================================*/
