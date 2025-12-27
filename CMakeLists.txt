cmake_minimum_required(VERSION 3.5)
project(ctask)

set(CMAKE_CXX_STANDARD 20)

add_executable(ctask
        main.cpp
        Task.cpp
        Task.h)

set_target_properties(ctask PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "{CMAKE_SOURCE_DIR}/bin"
)
