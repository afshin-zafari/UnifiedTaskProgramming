#ifndef __CHOLESKY_HPP__
#define __CHOLESKY_HPP__

#include "ductteip.hpp"
#include "math.h"

class DTGemm: public Algorithm{
private:
  DuctTeip_Data  *A,*B,*C;                          
  enum KernelKeys {GEMM};       
public:
  
  DTGemm(DuctTeip_Data *A_, DuctTeip_Data *B_, DuctTeip_Data  *C_ );

  void taskified();
  void runKernels(DuctTeip_Task *task );
  void  GEMM_kernel(DuctTeip_Task *dt_task);
  string getTaskName(unsigned long key);
  void  taskFinished(DuctTeip_Task *task, TimeUnit dur);
  void  checkCorrectness();
  void  populateMatrice();
  void  dumpAllData();
  ~DTGemm();
};
#endif //__CHOLESKY_HPP__
