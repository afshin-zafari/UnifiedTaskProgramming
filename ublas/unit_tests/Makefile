
outdir= ./temp
libfile=./lib/libublas.a
SUPERGLUE_DIR=../../../superglue/include
SUPERGLUE_FLAGS=-pthread -I$(SUPERGLUE_DIR)  #-pedantic -Wno-long-long -Wno-format
HEADER_DIR=./include
DUCTTEIP_FLAGS=-I$(HEADER_DIR)
CPP=mpic++
LINK=mpif90
#---------------------------------------------------------
WORKDIR = `pwd`

CC = mpicc
CXX = mpic++
AR = ar
LD = mpic++
WINDRES = windres

SOURCE_DIR=.
DUCTTEIP_DIR=/pica/h1/afshin/Damavand/D4/DuctTeip/ResStudio/Prototype

INC = -Iinclude -I../../dispatcher/include -I$(DUCTTEIP_DIR)/include -I/pica/h1/afshin/sg/superglue/include -I/pica/h1/afshin/Damavand/D3/sg_scheduler/include -I/pica/h1/afshin/Damavand/D3/cuBLAS_scheduler/include -I../include
CFLAGS = -Wall -fexceptions -std=c++11 -g -pthread
RESINC = 
LIBDIR = ../lib
LIB = ../lib/libublas.a ../../dispatcher/bin/Debug/libdispatcher.a ../../dt_scheduler/bin/Debug/libdt_scheduler.a ../../ductteip/bin/Debug/libductteip.a ../../cpu_blas_scheduler/bin/Debug/libcpu_blas_scheduler.a ../../sg_scheduler/bin/Debug/libsg_scheduler.a 
LDFLAGS = -L/pica/sw/comp/cuda/7.5/tintin/lib64 -lpthread -Wl,--allow-multiple-definition -lgsl -lgslcblas #-lcublas -lcudart
OBJDIR = ../obj
libfile=$(LIBDIR)/libublas.a
outdir=$(OBJDIR)
BINDIR=../bin

ifeq ($(CXX),icpc)
#-----------------------Intel Compiler set---------------------
	LINK_FLAGS=-lm -lrt 
	COMP_FLAGS= $(SUPERGLUE_FLAGS) $(DUCTTEIP_FLAGS) -std=c++11 -mavx  -opt-prefetch -Os -simd 
else
#-----------------------GCC Compiler set---------------------
	GCOV_FLAGS=-fprofile-arcs -ftest-coverage
	OPTIM_FLAGS=-mavx -march=bdver1 -mfma4 -Ofast  -Wwrite-strings
	SPECIAL_FLAGS=$(OPTIM_FLAGS)     
	LINK_FLAGS=-lm -lrt -lpthread
	COMP_FLAGS= $(SUPERGLUE_FLAGS) $(DUCTTEIP_FLAGS) -std=c++11 $(SPECIAL_FLAGS) 
endif
#########################################################
sources:=$(notdir $(shell ls -Sr $(SOURCE_DIR)/*.cpp))
objnames:=$(sources:%.cpp=%.o)
testnames:=$(sources:%.cpp=%.test)
sttestnames:=$(sources:%.cpp=%.sttest)

objects:=$(addprefix $(OBJDIR)/,$(objnames))	
tests:=$(addprefix $(BINDIR)/,$(testnames))	
sttests:=$(addprefix $(BINDIR)/,$(sttestnames))	

all: $(sttests)

$(objects):  $(outdir)/%.o:  $(SOURCE_DIR)/%.cpp 
	$(info compile $(notdir $<) )
	@$(CXX) -c -o $@ $< $(CFLAGS) $(INC)

$(sttests):  $(BINDIR)/%.sttest:  $(outdir)/%.o
	$(info build $(notdir $<) )
	$(LD)  -o $@ $<  $(LDFLAGS) $(LIB) 

clean: 
	rm -f $(BINDIR)/*.*test $(objects)
