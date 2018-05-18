cmake_minimum_required(VERSION 3.8)

project(unit_tests)

enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_BINARY_DIR}/build)

set(CPP_TESTS
  ${PROJECT_SOURCE_DIR}/src/queries.cpp
  ${PROJECT_SOURCE_DIR}/src/intersection.cpp
  ${PROJECT_SOURCE_DIR}/src/ball_bouncing.cpp
)

foreach(filename ${CPP_TESTS})

  # remove the filetype suffix
  string(REPLACE ".cpp" "_test" testname ${filename})
  
  # remove the prefix
  string(REPLACE ${PROJECT_SOURCE_DIR}/src/ "" testname ${testname})
  
  # use the remaining string to name the binary file
  add_executable(${testname} ${filename})
  add_test(${testname} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${testname})

  target_compile_features(${testname}
    PUBLIC
      cxx_std_11
  )

  target_link_libraries(${testname}
    bot_utils
  )
 
endforeach(filename ${CPP_TESTS})
