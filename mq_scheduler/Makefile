CC = gcc
CXX = g++
AR = ar
LD = g++

DUCTTEIP_DIR         = /home/ubuntu/UTP/UnifiedTaskProgramming/DuctTeip/ResStudio/Prototype
BOOST                = /home/ubuntu/boost/boost_1_61_0
RABIITMQ_TUROIAL_DIR = /home/ubuntu/RMQ/AMQP-CPP/examples/rabbitmq_tutorials
AMQPCPP_DIR          = /home/ubuntu/RMQ/AMQP-CPP
SUPERGLUE_DIR        = /home/ubuntu/UTP/UnifiedTaskProgramming/superglue

INC  = -I../dispatcher/include 
INC += -I../sg_scheduler/include 
INC += -I$(DUCTTEIP_DIR)/include 
INC += -I../dt_scheduler/include 
INC += -I../ublas/include
INC += -Iinclude 
INC += -I$(BOOST) 
INC += -I$(AMPQCPP_DIR)/include 
INC += -I$(RABIITMQ_TUROIAL_DIR) 
INC += -I$(RABIITMQ_TUROIAL_DIR)/include 
INC += -I$(RABIITMQ_TUROIAL_DIR)/build 
INC += -I$(SUPERGLUE_DIR)/include 

CFLAGS = -Wall -fexceptions  -Wno-unused-variable -std=c++11 -g -pthread -DSLIB=1 -DUTP_MQ
RESINC = 
LIBDIR =
LIB    = 

LDFLAGS  = -L$(AMQPCPP_DIR)/lib  
LDFLAGS += -L$(RABIITMQ_TUROIAL_DIR)/build  
LDFLAGS += -L$(BOOST)/stage/lib 
<<<<<<< HEAD
LDFLAGS += -Wl,-rpath,$(AMQP_CPP)/lib 
LDFLAGS += -L../dispatcher/bin/Debug -ldispatcher 
LDFLAGS += -Wl,--allow-multiple-definition 
LDFLAGS += -L../ductteip/bin/Debug -lductteip 
LDFLAGS += ../dt_scheduler/bin/Debug/libdt_scheduler.a 
LDFLAGS += ../sg_scheduler/bin/Debug/libsg_scheduler.a 
LDFLAGS += ../ublas/lib/libublas.a
LDFLAGS += ../cpu_blas_scheduler/bin/Debug/libcpu_blas_scheduler.a 
LDFLAGS += -lgsl -lgslcblas 
LDFLAGS += $(BOOST)/stage/lib/libboost_thread.a 
LDFLAGS += $(BOOST)/stage/lib/libboost_system.a 
LDFLAGS += -rdynamic 
LDFLAGS += -lamqpcpp 
LDFLAGS += -lasio_handler 
LDFLAGS += -lpthread  $
LDFLAGS += (BOOST)/stage/lib/libboost_system.a 
LDFLAGS += -Wl,-rpath,$(AMQPCPP_DIR)/lib 
LDFLAGS += -L../dispatcher/bin/Debug -ldispatcher 
LDFLAGS += -Wl,--allow-multiple-definition  

UGEMM_APP_LDFLAGS = -L$(AMQPCPP_DIR)/lib  
UGEMM_APP_LDFLAGS += -L$(RABIITMQ_TUROIAL_DIR)/build  
UGEMM_APP_LDFLAGS += -L$(BOOST)/stage/lib 
UGEMM_APP_LDFLAGS += -rdynamic 
UGEMM_APP_LDFLAGS += -lamqpcpp 
UGEMM_APP_LDFLAGS += -lasio_handler 
UGEMM_APP_LDFLAGS += -lpthread 
UGEMM_APP_LDFLAGS += $(BOOST)/stage/lib/libboost_system.a 
UGEMM_APP_LDFLAGS += $(BOOST)/stage/lib/libboost_thread.a 
UGEMM_APP_LDFLAGS += -Wl,-rpath,$(AMQPCPP_DIR)/lib 
UGEMM_APP_LDFLAGS += -L../dispatcher/bin/Debug 
UGEMM_APP_LDFLAGS += -lutpmq 
UGEMM_APP_LDFLAGS += -Wl,--allow-multiple-definition 
UGEMM_APP_LDFLAGS += -ldl 
UGEMM_APP_LDFLAGS += -L../ublas/lib -lublas


INC_DEBUG     = $(INC) 
CFLAGS_DEBUG  = $(CFLAGS) -g 
RESINC_DEBUG  = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG  = $(LIBDIR)
LIB_DEBUG     = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG  = obj/Debug
DEP_DEBUG     = 
OUT_DEBUG     = bin/Debug/mqapp

INC_RELEASE     = $(INC) 
CFLAGS_RELEASE  = $(CFLAGS) -O2 -D'BUILD=RELEASE'
RESINC_RELEASE  = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE  = $(LIBDIR)
LIB_RELEASE     = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE  = obj/Release
DEP_RELEASE     = 
OUT_RELEASE     = bin/Release/mqapp

UGEMM_APP_OBJ  = $(OBJDIR_DEBUG)/mq_scheduler.o 
UGEMM_APP_OBJ += $(OBJDIR_DEBUG)/MQWrapper.o 
UGEMM_APP_OBJ += $(OBJDIR_DEBUG)/ugemm_app.o


UGEMM_SERVER_OBJ  = $(OBJDIR_DEBUG)/send.o  
UGEMM_SERVER_OBJ += $(OBJDIR_DEBUG)/mq_scheduler.o 
UGEMM_SERVER_OBJ += $(OBJDIR_DEBUG)/MQWrapper.o 
UGEMM_SERVER_OBJ += $(OBJDIR_DEBUG)/ugemm_server.o

MQ_PROFILE_OBJ  = $(OBJDIR_DEBUG)/mq_scheduler.o 
MQ_PROFILE_OBJ += $(OBJDIR_DEBUG)/MQWrapper.o  
MQ_PROFILE_OBJ += $(OBJDIR_DEBUG)/mq_profile.o 

UGEMM_APP_OUT    = bin/Debug/ugemm_app
UGEMM_SERVER_OUT = bin/Debug/ugemm_server
MQ_PROFILE_OUT   = bin/Debug/mq_profile



OBJ_DEBUG  = $(OBJDIR_DEBUG)/send.o  
OBJ_DEBUG += $(OBJDIR_DEBUG)/mq_scheduler.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/MQWrapper.o

OBJ_RELEASE  = $(OBJDIR_RELEASE)/send.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/mq_scheduler.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/MQWrapper.o

all: debug release
clean: clean_debug clean_release

before_debug: 
	$(info ***************** MAIN APP (DEBUG) **************************)
	@test -d bin/Debug || mkdir -p bin/Debug
	@test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)
	@test -d $(OBJDIR_DEBUG)/src || mkdir -p $(OBJDIR_DEBUG)/src

after_debug: 
	$(info ************************************************************)

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG) $(UGEMM_SERVER_OBJ) $(MQ_PROFILE_OBJ)
	$(info creating $(OUT_DEBUG))
	@$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)
	$(info create ugemm_server)
	@$(LD) $(LIBDIR_DEBUG) -o $(UGEMM_SERVER_OUT) $(UGEMM_SERVER_OBJ) $(LDFLAGS_DEBUG) $(LIB_DEBUG)
	$(info create mq_profile)
	@$(LD) $(LIBDIR_DEBUG) -o $(MQ_PROFILE_OUT) $(MQ_PROFILE_OBJ)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)
ugemm_app:before_debug $(UGEMM_APP_OBJ) $(DEP_DEBUG) $(UGEMM_APP_OBJ)
	$(info create ugemm_app)
	@$(LD) $(LIBDIR_DEBUG) -o $(UGEMM_APP_OUT) $(UGEMM_APP_OBJ)  $(UGEMM_APP_LDFLAGS) $(LIB_DEBUG)


$(OBJDIR_DEBUG)/send.o: send.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c send.cpp -o $(OBJDIR_DEBUG)/send.o

$(OBJDIR_DEBUG)/mq_scheduler.o: src/mq_scheduler.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/mq_scheduler.cpp -o $(OBJDIR_DEBUG)/mq_scheduler.o

$(OBJDIR_DEBUG)/MQWrapper.o: src/MQWrapper.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/MQWrapper.cpp -o $(OBJDIR_DEBUG)/MQWrapper.o

$(OBJDIR_DEBUG)/ugemm_app.o: ugemm_app.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ugemm_app.cpp -o $(OBJDIR_DEBUG)/ugemm_app.o

$(OBJDIR_DEBUG)/ugemm_server.o: ugemm_server.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ugemm_server.cpp -o $(OBJDIR_DEBUG)/ugemm_server.o

$(OBJDIR_DEBUG)/mq_profile.o: mq_profile.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c mq_profile.cpp -o $(OBJDIR_DEBUG)/mq_profile.o


clean_debug: 
	@rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	@rm -rf bin/Debug
	@rm -rf $(OBJDIR_DEBUG)
	@rm -rf $(OBJDIR_DEBUG)/src

before_release: 
	$(info ***************** MAIN APP (RELEASE)************************)
	@test -d bin/Release || mkdir -p bin/Release
	@test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)
	@test -d $(OBJDIR_RELEASE)/src || mkdir -p $(OBJDIR_RELEASE)/src

after_release: 
	$(info ************************************************************)
	$(info )


release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(info creating $(OUT_RELEASE))
	@$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/send.o: send.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c send.cpp -o $(OBJDIR_RELEASE)/send.o

$(OBJDIR_RELEASE)/mq_scheduler.o: src/mq_scheduler.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/mq_scheduler.cpp -o $(OBJDIR_RELEASE)/mq_scheduler.o

$(OBJDIR_RELEASE)/MQWrapper.o: src/MQWrapper.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/MQWrapper.cpp -o $(OBJDIR_RELEASE)/MQWrapper.o

clean_release: 
	@rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	@rm -rf bin/Release
	@rm -rf $(OBJDIR_RELEASE)
	@rm -rf $(OBJDIR_RELEASE)/src

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

