CC = gcc
CXX = g++
AR = ar
LD = g++

DUCTTEIP_DIR         = /home/ubuntu/UTP/UnifiedTaskProgramming/DuctTeip/ResStudio/Prototype
BOOST                = /home/ubuntu/boost/boost_1_61_0
RABIITMQ_TUROIAL_DIR = /home/ubuntu/RMQ/AMQP-CPP/examples/rabbitmq_tutorials
AMQPCPP_DIR          = /home/ubuntu/RMQ/AMQP-CPP
CUDA                 = /pica/sw/comp/cuda/7.5/tintin

INC  = -Iinclude 
INC += -I../dt_scheduler/include 
INC += -I$(DUCTTEIP_DIR)/include 
INC += -I../superglue/include 
INC += -I../cpu_blas_scheduler/include 
INC += -I../sg_scheduler/include 
INC += -I../sg_scheduler/include 
INC += -I../cuBLAS_scheduler/include 
INC += -I../mq_scheduler/include 
INC += -I$(CUDA)/samples/common/inc 
INC += -I$(CUDA)/include -I../ublas/include 
INC += -I$(BOOST) 
INC += -I$(AMPQCPP_DIR)/include 
INC += -I$(RABIITMQ_TUROIAL_DIR) 
INC += -I$(RABIITMQ_TUROIAL_DIR)/include 
INC += -I$(RABIITMQ_TUROIAL_DIR)/build

CFLAGS  = -Wall -fexceptions -std=c++11 -pthread -Wno-unused-variable -DSLIB=1 -DUTP_MQ
RESINC  = 
LIBDIR  = 
LIB     = ../dt_scheduler/bin/Debug/libdt_scheduler.a ../sg_scheduler/bin/Debug/libsg_scheduler.a ../cpu_blas_scheduler/bin/Debug/libcpu_blas_scheduler.a
LDFLAGS = -Wl,--allow-multiple-definition  -lpthread 

INC_DEBUG     = $(INC) 
CFLAGS_DEBUG  = $(CFLAGS) -g
RESINC_DEBUG  = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG  = $(LIBDIR)
LIB_DEBUG     = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS) 
OBJDIR_DEBUG  = obj/Debug
DEP_DEBUG     = 
OUT_DEBUG     = bin/Debug/libdispatcher.a
UTPMQ_DEBUG   = bin/Debug/libutpmq.a

INC_RELEASE     = $(INC) 
CFLAGS_RELEASE  = $(CFLAGS) -O2 -D'BUILD=RELEASE'
RESINC_RELEASE  = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE  = $(LIBDIR)
LIB_RELEASE     = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE  = obj/Release
DEP_RELEASE     = 
OUT_RELEASE     = bin/Release/libdispatcher.a
UTPMQ_RELEASE   = bin/Release/libutpmq.a


OBJ_DEBUG  = $(OBJDIR_DEBUG)/src/memory_manager.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/src/IScheduler.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/src/GTask.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/src/GPartitioner.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/src/GOperation.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/src/GHandle.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/src/GData.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/src/GLog.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/src/Dispatcher.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/main.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/src/config.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/src/basic.o

UTPMQ_OBJ_DEBUG= $(subst Dispatcher,utpmq,$(OBJ_DEBUG))

OBJ_RELEASE  = $(OBJDIR_RELEASE)/src/memory_manager.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/src/IScheduler.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/src/GTask.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/src/GPartitioner.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/src/GOperation.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/src/GHandle.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/src/GData.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/src/GLog.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/src/Dispatcher.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/main.o 

UTPMQ_OBJ_RELEASE=$(subst Dispatcher,utpmq,$(OBJ_RELEASE))

all: debug release 

clean: clean_debug clean_release 


before_debug: 
	$(info ***************** DISPATCHER (DEBUG)************************)
	@test -d bin/Debug || mkdir -p bin/Debug
	@test -d $(OBJDIR_DEBUG)/src || mkdir -p $(OBJDIR_DEBUG)/src
	@test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug: 
	$(info ************************************************************)
	$(info )

debug: before_debug out_debug after_debug

.ONESHELL:
out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG) $(UTPMQ_OBJ_DEBUG)
	$(info creating $(OUT_DEBUG))
	@$(AR) rcsv $(OUT_DEBUG) $(OBJ_DEBUG)  | cat 
utpmq: before_debug $(DEP_DEBUG) $(UTPMQ_OBJ_DEBUG)
	$(info creating $(UTPMQ_DEBUG))
	@tput setaf 2 
	@$(AR) rcsv $(UTPMQ_DEBUG) $(UTPMQ_OBJ_DEBUG) | cat 
	@tput sgr0

$(OBJDIR_DEBUG)/src/memory_manager.o: src/memory_manager.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/memory_manager.cpp -o $(OBJDIR_DEBUG)/src/memory_manager.o

$(OBJDIR_DEBUG)/src/IScheduler.o: src/IScheduler.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/IScheduler.cpp -o $(OBJDIR_DEBUG)/src/IScheduler.o

$(OBJDIR_DEBUG)/src/GTask.o: src/GTask.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/GTask.cpp -o $(OBJDIR_DEBUG)/src/GTask.o

$(OBJDIR_DEBUG)/src/GPartitioner.o: src/GPartitioner.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/GPartitioner.cpp -o $(OBJDIR_DEBUG)/src/GPartitioner.o

$(OBJDIR_DEBUG)/src/GOperation.o: src/GOperation.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/GOperation.cpp -o $(OBJDIR_DEBUG)/src/GOperation.o

$(OBJDIR_DEBUG)/src/GHandle.o: src/GHandle.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/GHandle.cpp -o $(OBJDIR_DEBUG)/src/GHandle.o

$(OBJDIR_DEBUG)/src/GData.o: src/GData.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/GData.cpp -o $(OBJDIR_DEBUG)/src/GData.o

$(OBJDIR_DEBUG)/src/Dispatcher.o: src/Dispatcher.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Dispatcher.cpp -o $(OBJDIR_DEBUG)/src/Dispatcher.o

$(OBJDIR_DEBUG)/src/utpmq.o: src/Dispatcher.cpp
	$(info $(notdir $<) --> utpmq.o)
	@$(CXX) -DUTP_MQ $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Dispatcher.cpp -o $(OBJDIR_DEBUG)/src/utpmq.o

$(OBJDIR_DEBUG)/src/GLog.o: src/GLog.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/GLog.cpp -o $(OBJDIR_DEBUG)/src/GLog.o

$(OBJDIR_DEBUG)/src/config.o: $(DUCTTEIP_DIR)/src/config.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_DIR)/src/config.cpp -o $(OBJDIR_DEBUG)/src/config.o

$(OBJDIR_DEBUG)/src/basic.o: $(DUCTTEIP_DIR)/src/basic.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_DIR)/src/basic.cpp -o $(OBJDIR_DEBUG)/src/basic.o

$(OBJDIR_DEBUG)/main.o: main.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.cpp -o $(OBJDIR_DEBUG)/main.o

clean_debug: 
	@rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	@rm -rf bin/Debug
	@rm -rf $(OBJDIR_DEBUG)/src
	@rm -rf $(OBJDIR_DEBUG)

before_release: 
	$(info ***************** DISPATCHER (RELEASE)**********************)
	@test -d bin/Release || mkdir -p bin/Release
	@test -d $(OBJDIR_RELEASE)/src || mkdir -p $(OBJDIR_RELEASE)/src
	@test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release: 
	$(info ************************************************************)
	$(info )

release: before_release out_release after_release

.NOTPARALLEL:
out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE) $(UTPMQ_OBJ_RELEASE)
	$(info creating $(OUT_RELEASE))
	@$(AR) rcsv $(OUT_RELEASE) $(OBJ_RELEASE)
	$(info creating $(UTPMQ_RELEASE))
	@tput setaf 2
	@$(AR) rcsv $(UTPMQ_RELEASE) $(UTPMQ_OBJ_RELEASE) | cat 
	@tput sgr0

$(OBJDIR_RELEASE)/src/memory_manager.o: src/memory_manager.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/memory_manager.cpp -o $(OBJDIR_RELEASE)/src/memory_manager.o

$(OBJDIR_RELEASE)/src/IScheduler.o: src/IScheduler.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/IScheduler.cpp -o $(OBJDIR_RELEASE)/src/IScheduler.o

$(OBJDIR_RELEASE)/src/GTask.o: src/GTask.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/GTask.cpp -o $(OBJDIR_RELEASE)/src/GTask.o

$(OBJDIR_RELEASE)/src/GPartitioner.o: src/GPartitioner.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/GPartitioner.cpp -o $(OBJDIR_RELEASE)/src/GPartitioner.o

$(OBJDIR_RELEASE)/src/GOperation.o: src/GOperation.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/GOperation.cpp -o $(OBJDIR_RELEASE)/src/GOperation.o

$(OBJDIR_RELEASE)/src/GHandle.o: src/GHandle.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/GHandle.cpp -o $(OBJDIR_RELEASE)/src/GHandle.o

$(OBJDIR_RELEASE)/src/GData.o: src/GData.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/GData.cpp -o $(OBJDIR_RELEASE)/src/GData.o

$(OBJDIR_RELEASE)/src/Dispatcher.o: src/Dispatcher.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/Dispatcher.cpp -o $(OBJDIR_RELEASE)/src/Dispatcher.o

$(OBJDIR_RELEASE)/src/utpmq.o: src/Dispatcher.cpp
	$(info $(notdir $<))
	@$(CXX) -DUTP_MQ $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/Dispatcher.cpp -o $(OBJDIR_RELEASE)/src/utpmq.o

$(OBJDIR_RELEASE)/src/GLog.o: src/GLog.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/GLog.cpp -o $(OBJDIR_RELEASE)/src/GLog.o

$(OBJDIR_RELEASE)/main.o: main.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c main.cpp -o $(OBJDIR_RELEASE)/main.o

clean_release: 
	@rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	@rm -rf bin/Release
	@rm -rf $(OBJDIR_RELEASE)/src
	@rm -rf $(OBJDIR_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

