#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = mpicc
CXX = mpic++
AR = ar
LD = mpic++
WINDRES = windres
DUCTTEIP_DIR=/pica/h1/afshin/Damavand/D4/DuctTeip/ResStudio/Prototype

INC = -I../dispatcher/include -Iinclude -I../apps/include -I/usr/local/include -I$(DUCTTEIP_DIR)/include -I/pica/h1/afshin/sg/superglue/include -I/pica/h1/afshin/Damavand/D3/sg_scheduler/include -I../ublas/include
CFLAGS = -Wall -fexceptions -DSLIB=1 -std=c++11 
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = -lgsl -lgslcblas

INC_DEBUG = $(INC)  
CFLAGS_DEBUG = $(CFLAGS) -g 
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS) 
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/libcpu_blas_scheduler.a

INC_RELEASE = $(INC) 
CFLAGS_RELEASE = $(CFLAGS) -O2 -D'BUILD=RELEASE'
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = bin/Release/libcpu_blas_scheduler.a

OBJ_DEBUG = $(OBJDIR_DEBUG)/main.o $(OBJDIR_DEBUG)/src/CPUBLAS.o $(OBJDIR_DEBUG)/src/task_blas.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/main.o $(OBJDIR_RELEASE)/src/CPUBLAS.o $(OBJDIR_RELEASE)/src/task_blas.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	$(info ***************** CPU BLAS SCHEDULER (DEBUG)****************)
	@test -d bin/Debug || mkdir -p bin/Debug
	@test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)
	@test -d $(OBJDIR_DEBUG)/src || mkdir -p $(OBJDIR_DEBUG)/src

after_debug: 
	$(info ************************************************************)
	$(info )

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(info creating $(OUT_DEBUG))
	@$(AR) rcsv $(OUT_DEBUG) $(OBJ_DEBUG)  

$(OBJDIR_DEBUG)/main.o: main.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.cpp -o $(OBJDIR_DEBUG)/main.o

$(OBJDIR_DEBUG)/src/CPUBLAS.o: src/CPUBLAS.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/CPUBLAS.cpp -o $(OBJDIR_DEBUG)/src/CPUBLAS.o

$(OBJDIR_DEBUG)/src/task_blas.o: src/task_blas.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/task_blas.cpp -o $(OBJDIR_DEBUG)/src/task_blas.o

clean_debug: 
	@rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	@rm -rf bin/Debug
	@rm -rf $(OBJDIR_DEBUG)
	@rm -rf $(OBJDIR_DEBUG)/src

before_release: 
	$(info ***************** CPU BLAS SCHEDULER (RELEASE)**************)
	@test -d bin/Release || mkdir -p bin/Release
	@test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)
	@test -d $(OBJDIR_RELEASE)/src || mkdir -p $(OBJDIR_RELEASE)/src

after_release: 
	$(info ************************************************************)
	$(info )

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(info creating $(OUT_RELEASE))
	@$(AR) rcsv $(OUT_RELEASE) $(OBJ_RELEASE)  



$(OBJDIR_RELEASE)/main.o: main.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c main.cpp -o $(OBJDIR_RELEASE)/main.o

$(OBJDIR_RELEASE)/src/CPUBLAS.o: src/CPUBLAS.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/CPUBLAS.cpp -o $(OBJDIR_RELEASE)/src/CPUBLAS.o

$(OBJDIR_RELEASE)/src/task_blas.o: src/task_blas.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/task_blas.cpp -o $(OBJDIR_RELEASE)/src/task_blas.o

clean_release: 
	@rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	@rm -rf bin/Release
	@rm -rf $(OBJDIR_RELEASE)
	@rm -rf $(OBJDIR_RELEASE)/src

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

