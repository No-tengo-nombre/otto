file(GLOB UTILS_ROOT_SOURCE *.c)
file(GLOB UTILS_ROOT_HEADER *.h)
file(GLOB UTILS_VENDOR_SOURCE vendor/*.c)
file(GLOB UTILS_VENDOR_HEADER vendor/*.h)

add_library(${PROJECT_UTILS_NAME} ${UTILS_ROOT_HEADER} ${UTILS_ROOT_SOURCE}
                                  ${UTILS_VENDOR_HEADER} ${UTILS_VENDOR_SOURCE})
target_link_libraries(${PROJECT_UTILS_NAME} PUBLIC)
target_include_directories(
  ${PROJECT_UTILS_NAME} PUBLIC ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}
                               ${PROJECT_HEADERS} ${CMAKE_SOURCE_DIR} ${CMAKE_DEPENDENCIES_DIR}/OpenCL/include)
