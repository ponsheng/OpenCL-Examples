
function(cl_test name)
  set(cl_file ${CMAKE_CURRENT_SOURCE_DIR}/${name}.cl)
  set(new_header ${CMAKE_CURRENT_BINARY_DIR}/${name}_cl.h)

  add_custom_command(
    OUTPUT ${new_header}
    COMMAND cl2h ${CMAKE_CURRENT_SOURCE_DIR}/${name}.cl -o ${CMAKE_CURRENT_BINARY_DIR}

    DEPENDS cl2h ${cl_file}
    COMMENT "Building kernel header for '${name}'..."
    )

  # Save to parent scope list
  list(APPEND headers ${new_header})
  set(headers ${headers} PARENT_SCOPE)

  list(APPEND TEST_SRCS ${name}.c)
  set(TEST_SRCS ${TEST_SRCS} PARENT_SCOPE)

endfunction(cl_test)

cl_test(square)
cl_test(pipe)
cl_test(pipe_resv)

add_custom_target(cl_kernel_headers
  DEPENDS ${headers}
  )

add_library(TESTS
  OBJECT ${TEST_SRCS}
  )

include_directories(TESTS
  ${CMAKE_CURRENT_BINARY_DIR}
  )

add_dependencies(TESTS
  cl_kernel_headers
  )
