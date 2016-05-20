#ifndef CPUBLAS_HPP
#define CPUBLAS_HPP
#include "IScheduler.hpp"
#include "GTask.hpp"
#include "GData.hpp"
#include "Dispatcher.hpp"
#include "unified_blas.hpp"


class CPUBLAS : public IScheduler
{
public:
    CPUBLAS(int);
    virtual ~CPUBLAS();
    CPUBLAS(const CPUBLAS& other);
    CPUBLAS& operator=(const CPUBLAS& other);
    void submitTask(GTask *);
    void runTask(GTask *);
    void finishedTaskGroup(GTask *) {}
    void finishedTask(GTask *);
    void finalize();
    int init();
    void allocate_memory(GData *);
    void data_created(GData *);
    void data_partitioned(GData *);
protected:
private:
};

#endif // CPUBLAS_HPP
