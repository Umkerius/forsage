cmake_minimum_required(VERSION 3.0)

macro(enumerate_files_for_humans root_dir patterns output)
  foreach(pattern ${patterns})
    file(GLOB_RECURSE files "${root_dir}/${pattern}")
    list(APPEND ${output} ${files})
  endforeach()
endmacro()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(additional_cxx_flags "-std=c++14")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  set(additional_cxx_flags "-std=c++14")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Intel")
  set(additional_cxx_flags "-std=c++14")
elseif (MSVC)
  set (additional_cxx_flags "/std:c++14")
  set (additional_cxx_flags_debug "${additional_cxx_flags_debug} /MTd")
  set (additional_cxx_flags_release "${additional_cxx_flags_release} /MT")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${additional_cxx_flags}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${additional_cxx_flags_debug}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${additional_cxx_flags_release}")