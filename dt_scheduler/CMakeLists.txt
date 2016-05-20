cmake_minimum_required(VERSION 2.6)
project(dt_sceduler)

set(DUCTTEIP_DIR /pica/h1/afshin/Damavand/D4/DuctTeip/ResStudio/Prototype/include CACHE PATH "DuctTeip Path")
set(SUPERGLUE_DIR /pica/h1/afshin/sg/superglue/include CACHE PATH "SuperGlue Path")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/)


find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

add_definitions(-DWITH_MPI=0)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${DUCTTEIP_DIR} ${SUPERGLUE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../dispatcher/include ${CMAKE_CURRENT_SOURCE_DIR}/../sg_scheduler/include)
add_library(dt_scheduler src/DTWrapper.cpp)


