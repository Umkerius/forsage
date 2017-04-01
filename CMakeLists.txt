cmake_minimum_required(VERSION 2.8)
project(forsage)

include(cmake/common.cmake)
enable_testing()

# Forsage lib
include_directories("include")

set(header_patterns *.h *.hpp)
set(code_patterns *.c *.cc *.cpp *.cxx)
list(APPEND source_patterns ${header_patterns} ${code_patterns})

enumerate_files_for_humans("include" "${header_patterns}" forsage_includes)
enumerate_files_for_humans("src" "${source_patterns}" source_files)
enumerate_files_for_humans("src" "${header_patterns}" header_files)

source_group("sources" FILES ${source_files})
source_group("headers" FILES ${header_files})
source_group("forsage" FILES ${forsage_includes})

add_library(${PROJECT_NAME} STATIC ${source_files} ${header_files} ${forsage_includes})

# Forsage tests
# add Google Test framework dependency
add_subdirectory(googletest/googletest)
include_directories("${gtest_SOURCE_DIR}/include")
link_directories(${gtest_BINARY_DIR})

enumerate_files_for_humans("tests" "${source_patterns}" tests_sources)
source_group("tests" FILES ${tests_sources})

set(test_target "${PROJECT_NAME}_test")
add_executable(${test_target} ${tests_sources})
target_link_libraries(${test_target} ${PROJECT_NAME} gtest)
add_dependencies(${test_target} ${PROJECT_NAME} gtest)
add_test(${test_target} ${test_target})

message("additional_cxx_flags: ${additional_cxx_flags}")
message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message("CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
