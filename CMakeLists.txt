cmake_minimum_required(VERSION 3.0)
project(BCNN)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -mpopcnt")
set(CMAKE_C_COMPILER /usr/local/bin/gcc-7)

set(SOURCE_FILES
        main.c
        package/datatypes.h
        package/quantize_ops.h
        package/nn_ops.h
)
add_executable(BCNN ${SOURCE_FILES})