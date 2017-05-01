#include "GData.hpp"
#include "dispatcher.hpp"
#include "brdcast.hpp"
//#include "basic.hpp"
namespace utp{
std::vector<GData*> *all_data;
const int DATA_NAME_COPY=20;
/*=====================================================================*/
GData::GData()
{
    handle = new GHandle;
    if( all_data == nullptr)
        all_data = new std::vector<GData *>;
    key = all_data->size();
    all_data->push_back(this);
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
    if( all_data == nullptr)
        all_data = new std::vector<GData *>;
    key = all_data->size();
    all_data->push_back(this);
    
    memory      = nullptr;
    content     = nullptr;
    child_cnt   = 0;
    child_idx   = 0;
    parent      = nullptr;
    children    = nullptr;
    partitioner = nullptr;
    guest       = nullptr;
    level       = 0;
    
    BroadCast::data_created(this);
}
/*=====================================================================*/
GData::GData(int i,int j,string n,GData *p, int idx):
  M(i),N(j),child_idx(idx),parent(p),name(n)
{
    handle = new GHandle;
    if( all_data == nullptr)
        all_data = new std::vector<GData *>;
    key = all_data->size();
    all_data->push_back(this);
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
    BroadCast::data_created(this);
}
/*=====================================================================*/
string GData::get_name()
{
  return name;
}
/*=====================================================================*/
GData::~GData()
{
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
      PRINTF("Child data :%s of %dx%d at blk(%d,%d) is being created.\n",ch_name.c_str(),m,n,py,px);
      children[i] = new GData(m,n,ch_name,this,i);
    }
    BroadCast::data_partitioned(this);
    GPartitioner *next_p=partitioner->get_next();
    if ( next_p != nullptr)
      for(int i=0; i<child_cnt; i++)
	{
	  PRINTF("Cascade partition to %s with %dx%d\n",children[i]->get_name().c_str(),next_p->y,next_p->x);
	  children[i]->set_partition(next_p);
	}
}
/*=====================================================================*/
int GData::get_part_countY()
{
  if ( partitioner == nullptr )
    return 0;
  return partitioner->y;
}
/*=====================================================================*/
int GData::get_part_countX()
{
  if ( partitioner == nullptr )
    return 0;
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
//    content = m->getAddress();
    assert(content);
  }
}
/*=====================================================================*/
void GData::set_memory(void *m,int ld)
{
  content = (byte *)m;
  if ( ld == -1 )
    leading_dim = M;
  else
    leading_dim = ld;
  BroadCast::data_set_memory(this,(double*)m,leading_dim);
  PRINTF("set_memory for %s, m:%p , ld:%d\n",get_name().c_str(),m,ld);
  if ( child_cnt ==0 )
    return ;
  int bz = children[0]->get_rows() *   children[0]->get_cols();
  if ( m == nullptr )
    bz = 0;
  PRINTF("set_memory, m:%p , bz:%d\n",m,bz);
  for(int i=0; i<child_cnt; i++){
    children[i]->set_memory((double *)m+ i* bz,ld);
  }

  
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
int GData::get_ld()
{
    return leading_dim;
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
  void GData::fill_hilbert(){
    double *d = (double*)content;
    if(N>1000)
      return;
    if ( d != nullptr){
      for(int i=0;i<N;i++){
	for(int j=0;j<N;j++){
	  d[ j*N + i ]=1.0/(i+j+2-1);
	}
      }
    }
  }
/*=====================================================================*/
void GData::fill_with(double v,double inc)
{
  GPartitioner *p = partitioner;
  bool owner;
  double org_v=v;
  if ( p == nullptr )
    owner = true;
  else
    owner = p->is_owner(this);
  
  PRINTF("Fill with %s, mem:%p , %dx%d , ld:%d, owner:%d, v:%lf\n",get_name().c_str(), (double *)content, M,N,leading_dim, owner,v);
  double *d = (double*)content;
  if ( d != nullptr){
    PRINTF("###%p, result mem:%p\n",d,d+N*M);
    for(int j=0;j<N;j++){
      for(int i=0;i<M;i++){
	d[ j*M + i ]=v;
	v+=inc;
      }
    }
  }
  else if (owner){
    /*
    int z=get_element_count();
    set_memory( (void *)new double[z],M );
    v = org_v;
    d = (double*)content;
    PRINTF("^^^%p, length:%d, result mem:%p\n",d,(N-1)*leading_dim+M-1,d+(N-1)*leading_dim+M-1);
    for(int j=0;j<N;j++){
      for(int i=0;i<M;i++){
	d[ j*M + i ]=v;
	v+=inc;
      }
    } 
    */   
  }  
  v= org_v;
  for ( int c=0;c<child_cnt; c++){
    if ( children[c] != nullptr){
      children[c]->fill_with(v,inc);
    }
  }
}
/*=====================================================================*/
void GData::print()
{
  if ( M<16 ) {
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
      printf("%s with memory:%p lead_dim:%d has contents:\n",get_name().c_str(),content,leading_dim);
      for ( int i=0;i<M;i++){
	for ( int j=0;j<N;j++){
	  int ii=i/py;
	  int jj=j/px;
	  printf("%+02.2lf ",d[jj*py*px*by+ii*py*px+(j%px)*py+i%py]);
	  //printf("%+02.2lf ",d[j*M + i]);
	}
	printf("\n");
      }
    }
    else{
      printf("%s has no memory allocated.\n",get_name().c_str());
    }
  }
  
  if (child_cnt)
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
  if ( M > 12 ) return;
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
void GData::serialize(byte *buf, int &ofs)
{
    int invalid=-1;
    copy(buf,ofs,key);
    copy(buf,ofs,M);
    copy(buf,ofs,N);
    copy(buf,ofs,child_idx);
    copy(buf,ofs,child_cnt);
    copy(buf,ofs,level);
    if(parent){
      std::cout << "parent key of Data: " << parent->key << std::endl;
        copy(buf,ofs,parent->key);
    }
    else
        copy(buf,ofs,invalid);
    if (partitioner)
        partitioner->serialize(buf,ofs);
    else
        copy(buf,ofs,invalid);
    char s[DATA_NAME_COPY];
    strcpy(s,name.c_str());
    memcpy(buf+ofs,s, DATA_NAME_COPY);
    ofs +=DATA_NAME_COPY;
}
void GData::deserialize(byte *buf, int &ofs)
{
    int k,p,q,invalid=-1;
    paste(buf,ofs,&k);
    paste(buf,ofs,&M);
    paste(buf,ofs,&N);
    paste(buf,ofs,&child_idx);
    paste(buf,ofs,&child_cnt);
    paste(buf,ofs,&level);
    paste(buf,ofs,&p);
    if(p!=invalid  and (unsigned)p <all_data->size())
        parent = (*all_data)[p];
    else
        parent = nullptr;
    paste(buf,ofs,&q);
    if (q!=invalid)
        partitioner = DeserializePartitioner(buf,ofs);
    else
        partitioner = nullptr;
}
GData *DeserializeData(byte *buf, int &ofs)
{
    int k;
    paste(buf,ofs,&k);
    if(k>=0  and (unsigned)k <all_data->size())
        return (*all_data)[k];
    return nullptr;

}
GData* CreateData(byte *buf, int &ofs)
{
    int k,M,N,idx,cnt,level,p,q,invalid=-1;
    GData *d;
    paste(buf,ofs,&k);
    paste(buf,ofs,&M);
    paste(buf,ofs,&N);
    paste(buf,ofs,&idx);
    paste(buf,ofs,&cnt);
    paste(buf,ofs,&level);
    paste(buf,ofs,&p);
    paste(buf,ofs,&q);
    char s[DATA_NAME_COPY];
    memcpy(s,buf+ofs,DATA_NAME_COPY);
    ofs +=DATA_NAME_COPY;
    if(p!=invalid)
        d = new GData(M,N,s,(*all_data)[p],idx);
    else
        d = new GData(M,N,s);
    return d;
}
} // namespace utp
