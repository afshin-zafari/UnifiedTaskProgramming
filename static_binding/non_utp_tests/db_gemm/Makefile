

SUPERGLUE_DIR=/pica/h1/afshin/sg/superglue/include
SUPERGLUE_FLAGS=-pthread -I$(SUPERGLUE_DIR)  #-pedantic -Wno-long-long -Wno-format
HEADER_DIR=/pica/h1/afshin/Damavand/D4/DuctTeip/ResStudio/Prototype/include
DUCTTEIP_FLAGS=-I$(HEADER_DIR)
CPP=mpic++

ifeq ($(CXX),icpc)
#-----------------------Intel Compiler set---------------------
	ACML_DIR=/home/afshin/acml/acmllib/ifort64_fma4
	ACML_LIB=$(ACML_DIR)/lib/libacml.a
	EXPORT_PATH := export LD_LIBRARY_PATH='$(ACML_DIR)/lib:$(LD_LIBRARY_PATH)'
	BLAS_COMP_FLAGS=-I$(ACML_DIR)/include 

	LINKER=mpif90

	BLAS_LINK_FLAGS=-L$(ACML_DIR)/lib  -lacml -nofor-main
	LINK_FLAGS=-lm -lrt -no-ipo -lstdc++ $(BLAS_LINK_FLAGS) 
	COMP_FLAGS=-fast -no-ipo -opt-prefetch -Os -simd -mavx $(SUPERGLUE_FLAGS) $(DUCTTEIP_FLAGS) $(BLAS_COMP_FLAGS)
else
	ACML_DIR=/pica/h1/afshin/acml/gnu/acmllib/gfortran64
	ACML_LIB=$(ACML_DIR)/lib/libacml.a
	BLAS_COMP_FLAGS=-I$(ACML_DIR)/include 

	GCOV_FLAGS=-fprofile-arcs -ftest-coverage
	OPTIM_FLAGS=-mavx  -Wwrite-strings -march=bdver1 -O3 -mfma4
	SPECIAL_FLAGS=$(OPTIM_FLAGS)     


	LINKER=mpic++

	BLAS_LINK_FLAGS=-lgsl -lgslcblas
	LINK_FLAGS=-lm -lrt -march=bdver1 $(BLAS_LINK_FLAGS) -lstdc++ -lpthread -Wl,--allow-multiple-definition #-fprofile-arcs 
	COMP_FLAGS= -std=c++11 $(SUPERGLUE_FLAGS) $(DUCTTEIP_FLAGS) $(BLAS_COMP_FLAGS) $(SPECIAL_FLAGS)
endif
#########################################################
headers:=$(shell ls -Sr *.hpp)
DT_LIB_DIR=/pica/h1/afshin/Damavand/D3/ductteip/bin/Release


all: bin/non_utp_dt_blas

bin/non_utp_dt_blas: bin/dt_gemm.o bin/main.o
	$(info )
	$(info Link with DTLib.)
	$(LINKER) $? -L$(DT_LIB_DIR) -lductteip $(LINK_FLAGS) -pthread -lgsl -lgslcblas -o bin/non_utp_dt_blas 
	$(info )

bin/dt_gemm.o : dt_gemm.cpp $(headers)
	$(info )
	$(info Compile the source file.)
	$(CPP) -c dt_gemm.cpp -o bin/dt_gemm.o $(COMP_FLAGS)
	$(info )

bin/main.o : main.cpp $(headers)
	$(info )
	$(info Compile the source file.)
	$(CPP) -c main.cpp -o bin/main.o $(COMP_FLAGS)
	$(info )



clean: 
	rm -f bin/*.o bin/non_utp_dt_blas
