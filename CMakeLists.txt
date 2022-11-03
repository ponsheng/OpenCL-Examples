project(examples)

cmake_minimum_required(VERSION 3.16)

include_directories(
  "../OCL CTS/OpenCL-Headers"
  )

# Test target
add_subdirectory(utils)
add_subdirectory(tests)

find_library(OPENCL_LIB
  NAMES OpenCL
  PATHS "../OCL CTS/OpenCL-SDK-Release/lib"
  )
include_directories(
  tests
  )

if (OPENCL_LIB)
  add_executable(app
    main.c
    )
  target_link_libraries(app
    TESTS
    ${OPENCL_LIB}
  )
else (OPENCL_LIB)
  message("Warning: OpenCL library not found, skip...")
endif(OPENCL_LIB)
