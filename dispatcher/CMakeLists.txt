cmake_minimum_required(VERSION 2.6)
project(dispatcher)

set(DUCTTEIP_DIR /pica/h1/afshin/Damavand/D4/DuctTeip/ResStudio/Prototype/include CACHE PATH "DuctTeip Path")
set(SUPERGLUE_DIR /pica/h1/afshin/sg/superglue/include CACHE PATH "SuperGlue Path")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})



add_definitions(-DWITH_MPI=1)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${DUCTTEIP_DIR} ${SUPERGLUE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../sg_scheduler/include ${CMAKE_CURRENT_SOURCE_DIR}/../dt_scheduler/include ${CMAKE_CURRENT_SOURCE_DIR}/../cpu_blas_scheduler/include)
add_library(dispatcher src/Dispatcher.cpp  src/GData.cpp  src/GHandle.cpp  src/GLog.cpp  src/GOperation.cpp  src/GPartitioner.cpp  src/GTask.cpp  src/IScheduler.cpp  src/memory_manager.cpp)


