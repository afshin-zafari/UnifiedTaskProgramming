#ifndef __MEM_MANAGER_HPP__
#define  __MEM_MANAGER_HPP__
#include "utp_basic.hpp"
#include <vector>
namespace utp{
typedef unsigned long MemoryKey;
/*----------------------------------------------------------------------------*/
struct MemoryItem{
private:
  MemoryKey     key;
  ulong  	size;
  byte         *address;
  int           state;
public:
  enum MemoryState{
    Initialized,
    Allocated,
    InUse,
    Ready,
    Deallocated
  };

/*----------------------------------------------------------------------------*/
  MemoryItem () ;
  MemoryItem(int size_,MemoryKey key_=0);
  ~MemoryItem();
  void setState(int s ) ;
  MemoryKey getKey() ;
  unsigned long getSize() ;
  int getState() ;
  byte *getAddress() ;
  void dump();
};
/*============================================================================*/

/*============================================================================*/

class MemoryManager{
private:
  std::vector <MemoryItem *> memory_list;
  unsigned long       element_size;
  MemoryKey           last_key;
public:
/*----------------------------------------------------------------------------*/
  MemoryManager(int elem_count, int elem_size);
  ~MemoryManager();
  MemoryItem *getNewMemory();
  MemoryItem *expandMemoryItems(int n =0);
  MemoryItem *findFreeMemory();
  void freeMemoryItem(MemoryItem *m ) ;

};
/*============================================================================*/
}
#endif // __MEM_MANAGER_HPP__
