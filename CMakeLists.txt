project(examples)

cmake_minimum_required(VERSION 3.16)

find_library(OPENCL_LIB
  NAMES OpenCL
  PATHS "../OCL CTS/OpenCL-SDK-Release/lib"
  )

if (OPENCL_LIB)
  add_executable(app
    hello.c
    square.c
    pipe.c
    pipe_resv.c
    )

  include_directories(app
    "../OCL CTS/OpenCL-Headers"
    )

  target_link_libraries(app
    ${OPENCL_LIB}
  )
else (OPENCL_LIB)
  message("Warning: OpenCL library not found, skip...")
endif(OPENCL_LIB)

add_subdirectory(utils)

