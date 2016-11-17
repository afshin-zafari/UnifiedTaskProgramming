
CC  = gcc
CXX = g++
AR  = ar
LD  = g++


INC     = -I../../include 
CFLAGS  = -Wall -fexceptions -std=c++11
RESINC  = 
LIBDIR  = 
LIB     = 
LDFLAGS = -lpthread -lgsl -lgslcblas

INC_DEBUG     = $(INC) 
CFLAGS_DEBUG  = $(CFLAGS) -g -pthread
RESINC_DEBUG  = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG  = $(LIBDIR)
LIB_DEBUG     = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG  = obj/Debug
DEP_DEBUG     = 
OUT_DEBUG     = bin/Debug/non_utp_blas_only

INC_RELEASE     = $(INC)
CFLAGS_RELEASE  = $(CFLAGS) -O2
RESINC_RELEASE  = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE  = $(LIBDIR)
LIB_RELEASE     = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE  = obj/Release
DEP_RELEASE     = 
OUT_RELEASE     = bin/Release/non_utp_blas_only

OBJ_DEBUG = $(OBJDIR_DEBUG)/main.o $(OBJDIR_DEBUG)/utp_basic.o $(OBJDIR_DEBUG)/cmdline.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/main.o $(OBJDIR_RELEASE)/utp_basic.o $(OBJDIR_RELEASE)/cmdline.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/main.o: main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.cpp -o $(OBJDIR_DEBUG)/main.o

$(OBJDIR_DEBUG)/utp_basic.o: ../../src/utp_basic.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../src/utp_basic.cpp -o $(OBJDIR_DEBUG)/utp_basic.o

$(OBJDIR_DEBUG)/cmdline.o: ../../src/cmdline.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../src/cmdline.cpp -o $(OBJDIR_DEBUG)/cmdline.o



clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/main.o: main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c main.cpp -o $(OBJDIR_RELEASE)/main.o

$(OBJDIR_RELEASE)/utp_basic.o: ../../src/utp_basic.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../src/utp_basic.cpp -o $(OBJDIR_RELEASE)/utp_basic.o

$(OBJDIR_RELEASE)/cmdline.o: ../../src/cmdline.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../src/cmdline.cpp -o $(OBJDIR_RELEASE)/cmdline.o


clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

