cmake_minimum_required(VERSION 3.23)

# Compiler and generator configurations
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
if(MSVC)
  set(PROJECT_COMPILE_OPTIONS /WX /W4)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
else()
  set(PROJECT_COMPILE_OPTIONS
      -Wall
      -Wextra
      -Werror
      -Wno-unused-parameter
      -Wno-unused-function
      -Wconversion
      -Wsign-conversion
      -pedantic
      -pedantic-errors)
endif()

# Project setup
project(otto VERSION 0.1)
set(CMAKE_DEPENDENCIES_DIR ${CMAKE_SOURCE_DIR}/dependencies)

# Necessary libraries
find_package(OpenCL REQUIRED)

# Output for files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/pdb)
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/pdb)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/debug)
set(CMAKE_PDB_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/pdb/debug)
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/pdb/debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_PDB_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/pdb)
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/pdb)

# Directory of the code
set(PROJECT_SOURCE_DIR_LOCATION ${CMAKE_SOURCE_DIR}/src)
seT(PROJECT_HEADERS_DEST "include")

# Names for the different libraries in the project
set(PROJECT_CORE_NAME ${PROJECT_NAME}_core)
set(PROJECT_UTILS_NAME ${PROJECT_NAME}_utils)
set(PROJECT_HEADERS ${CMAKE_SOURCE_DIR}/include)
set(PROJECT_CL_KERNELS ${CMAKE_SOURCE_DIR}/cl)

option(OTTO_BUILD_EXAMPLES "Build the examples that come with the package."
       OFF)
option(OTTO_BUILD_TESTS "Build the tests that come with the package." OFF)
option(OTTO_INSTALL "Install target." OFF)
option(BUILD_SHARED_LIBS "Build using shared libraries." ON)

# Compile definitions
add_compile_definitions(OTTO_CLKERNELS_DIR="${PROJECT_CL_KERNELS}")
add_compile_definitions(LOG_USE_COLOR)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  add_compile_definitions(OTTO_BUILD_TYPE_RELEASE)
  # add_compile_definitions(OTTO_DISABLE_LOGGING)
else()
  add_compile_definitions(OTTO_BUILD_TYPE_DEBUG)
endif()

string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")

# Subdirectories

add_subdirectory(dependencies)
add_subdirectory(src)

if(${OTTO_BUILD_EXAMPLES})
  add_subdirectory(examples)
endif()

if(${OTTO_BUILD_TESTS})
  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()

if(${OTTO_INSTALL})
  # libraries
  install(TARGETS ${PROJECT_CORE_NAME} ${PROJECT_UTILS_NAME})

  # headers
  file(GLOB BASE_HEADERS ${PROJECT_HEADERS}/otto/*.h)
  file(GLOB BASE_CL_HEADERS ${PROJECT_HEADERS}/otto/cl/*.h)
  file(GLOB UTILS_HEADERS ${PROJECT_HEADERS}/ottou/*.h)
  install(FILES ${BASE_HEADERS} DESTINATION ${PROJECT_HEADERS_DEST}/otto)
  install(FILES ${BASE_CL_HEADERS} DESTINATION ${PROJECT_HEADERS_DEST}/otto/cl)
  install(FILES ${UTILS_HEADERS} DESTINATION ${PROJECT_HEADERS_DEST}/ottou)
endif()

