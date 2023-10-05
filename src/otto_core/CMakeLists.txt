file(GLOB CORE_ROOT_SOURCE *.c)
file(GLOB CORE_ROOT_HEADER *.h)
file(GLOB CORE_CL_SOURCE cl/*.c)
file(GLOB CORE_CL_HEADER cl/*.h)

add_library(${PROJECT_CORE_NAME} ${CORE_ROOT_HEADER} ${CORE_ROOT_SOURCE} ${CORE_CL_HEADER} ${CORE_CL_SOURCE})

target_link_libraries(${PROJECT_CORE_NAME} PUBLIC ${PROJECT_UTILS_NAME}
                                                  OpenCL::OpenCL)
target_include_directories(
  ${PROJECT_CORE_NAME}
  PUBLIC ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}
         ${PROJECT_SOURCE_DIR_LOCATION} ${PROJECT_HEADERS} ${CMAKE_SOURCE_DIR}
         ${CMAKE_DEPENDENCIES_DIR}/OpenCL/include
         ${CMAKE_DEPENDENCIES_DIR}/uthash/src)
