#include "memory_manager.hpp"
namespace utp{

/*----------------------------------------------------------------------------*/
MemoryItem::MemoryItem ()
{
    address = nullptr;
    size    = 0;
    key     = -1;
    state   = Deallocated;
}
MemoryItem::MemoryItem(int size_,MemoryKey key_)
{
    address = new byte[size_];
    state = Ready;
    size = size_;
    key = key_;
    dump();
}
/*----------------------------------------------------------------------------*/
MemoryItem::~MemoryItem()
{
    delete[] address;
}
/*----------------------------------------------------------------------------*/
void MemoryItem::setState(int s )
{
    state = s;
}
/*----------------------------------------------------------------------------*/
MemoryKey MemoryItem::getKey()
{
    return key;
}
/*----------------------------------------------------------------------------*/
unsigned long MemoryItem::getSize()
{
    return size;
}
/*----------------------------------------------------------------------------*/
int MemoryItem::getState()
{
    return state;
}
/*----------------------------------------------------------------------------*/
byte *MemoryItem::getAddress()
{
    return address;
}
/*----------------------------------------------------------------------------*/
void MemoryItem::dump()
{
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/

/*============================================================================*/

/*----------------------------------------------------------------------------*/
MemoryManager::MemoryManager(int elem_count, int elem_size)
{
    element_size = elem_size;

    last_key = 0 ;
    expandMemoryItems(elem_count);
}
/*----------------------------------------------------------------------------*/
MemoryManager::~MemoryManager()
{
    memory_list.clear();
}

/*----------------------------------------------------------------------------*/
MemoryItem *MemoryManager::getNewMemory()
{
    MemoryItem *m = nullptr;
    m = findFreeMemory();
    if ( m == nullptr )
    {
        expandMemoryItems();
        m = findFreeMemory();
    }
    m->setState(MemoryItem::InUse);
    return m;
}
/*----------------------------------------------------------------------------*/
MemoryItem *MemoryManager::expandMemoryItems(int n )
{
    MemoryItem * first_mem,*m;
    first_mem = (MemoryItem *)0;
    int elem_count = memory_list.size();
    int expand  = (elem_count ==0)?n:elem_count/2 + 1;
    for ( int i = 0 ; i < expand; i ++)
    {
        m = new MemoryItem(element_size,last_key ++);
        if ( i == 0 )
            first_mem = m ;
        memory_list.push_back(m);
    }
    return first_mem;
}
/*----------------------------------------------------------------------------*/
MemoryItem *MemoryManager::findFreeMemory()
{
    MemoryItem *m = nullptr;
    for(auto m: memory_list)
    {
        if ( m->getState() != MemoryItem::InUse )
        {
            return m;
        }
    }
    return m;
}
/*----------------------------------------------------------------------------*/
void MemoryManager::freeMemoryItem(MemoryItem *m )
{
    m->setState(MemoryItem::Ready);
}
}
