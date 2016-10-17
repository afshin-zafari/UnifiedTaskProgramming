
outdir= ./temp
libfile=./lib/libublas.a
SUPERGLUE_FLAGS=-pthread -I$(SUPERGLUE_DIR)  #-pedantic -Wno-long-long -Wno-format
SOURCE_DIR=./src
HEADER_DIR=./include
DUCTTEIP_FLAGS=-I$(HEADER_DIR)
CPP=mpic++
LINK=mpif90
#---------------------------------------------------------
CC =gcc
CXX = g++
AR = ar
LD = g++
DUCTTEIP_DIR=/home/ubuntu/UTP/UnifiedTaskProgramming/DuctTeip/ResStudio/Prototype
SUPERGLUE_DIR=/home/ubuntu/UTP/UnifiedTaskProgramming/superglue

INC = -Iinclude 
INC += -I../dispatcher/include 
INC += -I$(DUCTTEIP_DIR)/include 
INC += -I$(SUPERGLUE_DIR)/include 
INC += -I../sg_scheduler/include 
INC += -I../cuBLAS_scheduler/include

CFLAGS = -Wall -fexceptions -std=c++11 -g -pthread -DSLIB=1 -DUTP_MQ
RESINC = 
LIBDIR = ./lib
LIB  = ../dispatcher/bin/Debug/libdispatcher.a 
LIB += ../dt_scheduler/bin/Debug/libdt_scheduler.a 
LIB += s../ductteip/bin/Debug/libductteip.a ../cpu_blas_scheduler/bin/Debug/libcpu_blas_scheduler.a ../sg_scheduler/bin/Debug/libsg_scheduler.a
LDFLAGS = -L/pica/sw/comp/cuda/7.5/tintin/lib64 -lpthread -Wl,--allow-multiple-definition -lgsl -lgslcblas #-lcublas -lcudart
OBJDIR = ./obj
libfile=$(LIBDIR)/libublas.a
outdir=$(OBJDIR)

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
objects:=$(addprefix $(OBJDIR)/,$(objnames))	
all: $(libfile)

$(libfile) : $(libfile)($(outdir)/*.o)
	$(info  )
	$(info ---------------------------------------------------------)
	$(info renew library '$(libfile)'.)
	$(info ---------------------------------------------------------)
	$(info  )
	#@ranlib $(libfile)

$(libfile)($(outdir)/*.o) : $(objects)
	$(info  )
	$(info ---------------------------------------------------------)
	$(info add objects to library. )
	$(info ---------------------------------------------------------)
	$(info  )
	@ar rcsv $(libfile) $?

$(objects):  $(outdir)/%.o:  $(SOURCE_DIR)/%.cpp $(HEADER_DIR)/%.hpp
	$(info compile $(notdir $<) )
	@$(CXX) -c -o $@ $< $(CFLAGS) $(INC)





clean: 
	rm $(outdir)/*.o $(libfile)
