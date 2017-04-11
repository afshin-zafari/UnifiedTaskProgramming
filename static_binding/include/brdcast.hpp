#ifndef BRDCAST_HPP
#define BRDCAST_HPP
#ifdef SPU_INCLUDED
#include "sch_spu.hpp"
#endif
#ifdef SG_INCLUDED
#include "sch_superglue.hpp"
#endif
#ifdef DT_INCLUDED
#include "sch_ductteip.hpp"
#endif

namespace utp{
    class BroadCast{
    public:
      static void data_created(GData *d){
#ifdef SG_INCLUDED
	SG::data_created(d);
#endif // SG_INCLUDED
#ifdef DT_INCLUDED
	DT::data_created(d);
#endif // DT_INCLUDED
#ifdef SPU_INCLUDED
	SPU::data_created(d);
#endif
      }
      static void data_set_memory(GData *d, double *mem, int ld){
#ifdef SPU_INCLUDED
	SPU::data_set_memory(d,mem,ld);
#endif
      }
      static void data_partitioned(GData *d){
#ifdef SPU_INCLUDED
	SPU::data_partitioned(d);
#endif
#ifdef SG_INCLUDED
	SG::data_partitioned(d);
#endif // SG_INCLUDED
#ifdef DT_INCLUDED
	DT::data_partitioned(d);
#endif // DT_INCLUDED
      }
      static void partition_defined(GPartitioner *p){
#ifdef SG_INCLUDED
	SG::partition_defined(p);
#endif // SG_INCLUDED
#ifdef DT_INCLUDED
	DT::partition_defined(p);
#endif // DT_INCLUDED
#ifdef SPU_INCLUDED
	SPU::partition_defined(p);
#endif
      }
      static void partition_cascaded(GPartitioner *p1, GPartitioner *p2){
#ifdef SG_INCLUDED
	SG::partition_cascaded(p1,p2);
#endif // SG_INCLUDED
#ifdef DT_INCLUDED
	DT::partition_cascaded(p1,p2);
#endif // DT_INCLUDED
#ifdef SPU_INCLUDED
	SPU::partition_cascaded(p1,p2);
#endif
      }

    };
}

#endif // BRDCAST_HPP
