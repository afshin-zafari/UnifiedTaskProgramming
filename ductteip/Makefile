CMP = mpiicpc
CC  = $(CMP)
CXX = $(CMP)
AR  = ar
LD  = $(CMP)

OBJ_OUT        = __/o
#DUCTTEIP_DIR  = /pica/h1/afshin/Damavand/D4/DuctTeip/ResStudio/Prototype
DUCTTEIP_DIR   = /home/a/afshin/src/DuctTeip/ResStudio/Prototype
DUCTTEIP_SRC   = $(DUCTTEIP_DIR)/src
#SUPERGLUE_DIR = /pica/h1/afshin/sg/superglue
SUPERGLUE_DIR  = /home/a/afshin/src/superglue

INC     = -I$(DUCTTEIP_DIR)/include -I$(SUPERGLUE_DIR)/include
CFLAGS  = -Wall -fexceptions
RESINC  = 
LIBDIR  = 
LIB     = 
LDFLAGS = -Wl,--allow-multiple-definition 

INC_DEBUG     = $(INC) 
CFLAGS_DEBUG  = $(CFLAGS) -g -std=c++11 -DBUILD=DEBUG
RESINC_DEBUG  = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG  = $(LIBDIR)
LIB_DEBUG     = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG  = obj/Debug
DEP_DEBUG     = 
OUT_DEBUG     = bin/Debug/libductteip.a

INC_RELEASE     = $(INC)
CFLAGS_RELEASE  = $(CFLAGS) -O2 -D'BUILD=RELEASE' -std=c++11
RESINC_RELEASE  = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE  = $(LIBDIR)
LIB_RELEASE     = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE  = obj/Release
DEP_RELEASE     = 
OUT_RELEASE     = bin/Release/libductteip.a



OBJ_DEBUG  = $(OBJDIR_DEBUG)/$(OBJ_OUT)/hostpolicy.o  
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/procgrid.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/partition.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/network.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/mpi_comm.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/memory_manager.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/mailbox.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/listener.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/glb_context.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/engine.o 

OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_data_lis.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_memory.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_process.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_task.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_term.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_thread.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_work.o 

OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/ductteip.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/dt_task.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/dt_log.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/dlb.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/distribute.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/data_basic.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/data.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/context.o  
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/config.o 
OBJ_DEBUG += $(OBJDIR_DEBUG)/$(OBJ_OUT)/basic.o

OBJ_RELEASE  = $(OBJDIR_RELEASE)/$(OBJ_OUT)/hostpolicy.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/procgrid.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/partition.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/network.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/mpi_comm.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/memory_manager.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/mailbox.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/listener.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/glb_context.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/engine.o 

OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_data_lis.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_memory.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_process.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_task.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_term.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_thread.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_work.o 

OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/ductteip.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/dt_task.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/dt_log.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/dlb.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/distribute.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/data_basic.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/data.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/context.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/config.o 
OBJ_RELEASE += $(OBJDIR_RELEASE)/$(OBJ_OUT)/basic.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	$(info ***************** DUCTTEIP FRAMEWORK (DEBUG)****************)
	@test -d bin/Debug || mkdir -p bin/Debug
	@test -d $(OBJDIR_DEBUG)/$(OBJ_OUT) || mkdir -p $(OBJDIR_DEBUG)/$(OBJ_OUT)
	@test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug: 
	$(info ************************************************************)
	$(info )

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(info creating $(OUT_DEBUG))
	@$(AR) rcsv $(OUT_DEBUG) $(OBJ_DEBUG) 


$(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_data_lis.o: $(DUCTTEIP_SRC)/eng_data_lis.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/eng_data_lis.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_data_lis.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_memory.o: $(DUCTTEIP_SRC)/eng_memory.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/eng_memory.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_memory.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_process.o: $(DUCTTEIP_SRC)/eng_process.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/eng_process.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_process.o
$(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_task.o: $(DUCTTEIP_SRC)/eng_task.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/eng_task.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_task.o
$(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_term.o: $(DUCTTEIP_SRC)/eng_term.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/eng_term.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_term.o
$(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_thread.o: $(DUCTTEIP_SRC)/eng_thread.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/eng_thread.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_thread.o
$(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_work.o: $(DUCTTEIP_SRC)/eng_work.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/eng_work.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/eng_work.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/hostpolicy.o: $(DUCTTEIP_SRC)/hostpolicy.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/hostpolicy.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/hostpolicy.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/procgrid.o: $(DUCTTEIP_SRC)/procgrid.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/procgrid.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/procgrid.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/partition.o: $(DUCTTEIP_SRC)/partition.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/partition.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/partition.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/network.o: $(DUCTTEIP_SRC)/network.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/network.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/network.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/mpi_comm.o: $(DUCTTEIP_SRC)/mpi_comm.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/mpi_comm.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/mpi_comm.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/memory_manager.o: $(DUCTTEIP_SRC)/memory_manager.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/memory_manager.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/memory_manager.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/mailbox.o: $(DUCTTEIP_SRC)/mailbox.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/mailbox.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/mailbox.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/listener.o: $(DUCTTEIP_SRC)/listener.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/listener.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/listener.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/glb_context.o: $(DUCTTEIP_SRC)/glb_context.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/glb_context.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/glb_context.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/engine.o: $(DUCTTEIP_SRC)/engine.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/engine.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/engine.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/ductteip.o: $(DUCTTEIP_SRC)/ductteip.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/ductteip.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/ductteip.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/dt_task.o: $(DUCTTEIP_SRC)/dt_task.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/dt_task.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/dt_task.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/dt_log.o: $(DUCTTEIP_SRC)/dt_log.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/dt_log.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/dt_log.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/dlb.o: $(DUCTTEIP_SRC)/dlb.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/dlb.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/dlb.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/distribute.o: $(DUCTTEIP_SRC)/distribute.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/distribute.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/distribute.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/data_basic.o: $(DUCTTEIP_SRC)/data_basic.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/data_basic.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/data_basic.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/data.o: $(DUCTTEIP_SRC)/data.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/data.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/data.o

$(OBJDIR_DEBUG)/main.o: main.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.cpp -o $(OBJDIR_DEBUG)/main.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/context.o: $(DUCTTEIP_SRC)/context.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/context.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/context.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/config.o: $(DUCTTEIP_SRC)/config.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/config.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/config.o

$(OBJDIR_DEBUG)/$(OBJ_OUT)/basic.o: $(DUCTTEIP_SRC)/basic.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $(DUCTTEIP_SRC)/basic.cpp -o $(OBJDIR_DEBUG)/$(OBJ_OUT)/basic.o

clean_debug: 
	@rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	@rm -rf bin/Debug
	@rm -rf $(OBJDIR_DEBUG)/$(OBJ_OUT)
	@rm -rf $(OBJDIR_DEBUG)
before_release: 
	$(info ***************** DUCTTEIP FRAMEWORK (RELEASE)***************)
	@test -d bin/Release || mkdir -p bin/Release
	@test -d $(OBJDIR_RELEASE)/$(OBJ_OUT) || mkdir -p $(OBJDIR_RELEASE)/$(OBJ_OUT)
	@test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)
after_release: 
	$(info ************************************************************)
	$(info )

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(info creating $(OUT_RELEASE))
	@$(AR) rcsv $(OUT_RELEASE) $(OBJ_RELEASE) 

$(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_data_lis.o: $(DUCTTEIP_SRC)/eng_data_lis.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/eng_data_lis.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_data_lis.o
$(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_memory.o: $(DUCTTEIP_SRC)/eng_memory.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/eng_memory.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_memory.o
$(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_process.o: $(DUCTTEIP_SRC)/eng_process.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/eng_process.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_process.o
$(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_task.o: $(DUCTTEIP_SRC)/eng_task.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/eng_task.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_task.o
$(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_term.o: $(DUCTTEIP_SRC)/eng_term.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/eng_term.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_term.o
$(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_thread.o: $(DUCTTEIP_SRC)/eng_thread.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/eng_thread.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_thread.o
$(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_work.o: $(DUCTTEIP_SRC)/eng_work.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/eng_work.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/eng_work.o



$(OBJDIR_RELEASE)/$(OBJ_OUT)/hostpolicy.o: $(DUCTTEIP_SRC)/hostpolicy.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/hostpolicy.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/hostpolicy.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/procgrid.o: $(DUCTTEIP_SRC)/procgrid.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/procgrid.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/procgrid.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/partition.o: $(DUCTTEIP_SRC)/partition.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/partition.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/partition.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/network.o: $(DUCTTEIP_SRC)/network.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/network.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/network.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/mpi_comm.o: $(DUCTTEIP_SRC)/mpi_comm.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/mpi_comm.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/mpi_comm.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/memory_manager.o: $(DUCTTEIP_SRC)/memory_manager.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/memory_manager.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/memory_manager.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/mailbox.o: $(DUCTTEIP_SRC)/mailbox.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/mailbox.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/mailbox.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/listener.o: $(DUCTTEIP_SRC)/listener.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/listener.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/listener.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/glb_context.o: $(DUCTTEIP_SRC)/glb_context.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/glb_context.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/glb_context.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/engine.o: $(DUCTTEIP_SRC)/engine.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/engine.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/engine.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/ductteip.o: $(DUCTTEIP_SRC)/ductteip.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/ductteip.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/ductteip.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/dt_task.o: $(DUCTTEIP_SRC)/dt_task.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/dt_task.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/dt_task.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/dt_log.o: $(DUCTTEIP_SRC)/dt_log.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/dt_log.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/dt_log.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/dlb.o: $(DUCTTEIP_SRC)/dlb.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/dlb.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/dlb.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/distribute.o: $(DUCTTEIP_SRC)/distribute.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/distribute.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/distribute.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/data_basic.o: $(DUCTTEIP_SRC)/data_basic.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/data_basic.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/data_basic.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/data.o: $(DUCTTEIP_SRC)/data.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/data.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/data.o

$(OBJDIR_RELEASE)/main.o: main.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c main.cpp -o $(OBJDIR_RELEASE)/main.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/context.o: $(DUCTTEIP_SRC)/context.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/context.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/context.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/config.o: $(DUCTTEIP_SRC)/config.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/config.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/config.o

$(OBJDIR_RELEASE)/$(OBJ_OUT)/basic.o: $(DUCTTEIP_SRC)/basic.cpp
	$(info $(notdir $<))
	@$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $(DUCTTEIP_SRC)/basic.cpp -o $(OBJDIR_RELEASE)/$(OBJ_OUT)/basic.o

clean_release: 
	@rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	@rm -rf bin/Release
	@rm -rf $(OBJDIR_RELEASE)/$(OBJ_OUT)
	@rm -rf $(OBJDIR_RELEASE)
.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

