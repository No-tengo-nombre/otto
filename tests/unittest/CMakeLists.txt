file(GLOB TESTS_QUICK quick/*.c)

# QUICK TESTS #
foreach(filepath ${TESTS_QUICK})
  # Get useful names
  file(RELATIVE_PATH filename ${CMAKE_CURRENT_SOURCE_DIR}/quick ${filepath})
  get_filename_component(filename_we ${filename} NAME_WE)
  set(filename_we test.${filename_we})

  # Compile the test
  add_executable(${filename_we} ${filepath})
  target_link_libraries(${filename_we} PUBLIC ${PROJECT_CORE_NAME}
                                              ${PROJECT_UTILS_NAME})
  target_include_directories(
    ${filename_we} PUBLIC ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}
                          ${PROJECT_HEADERS} ${CMAKE_SOURCE_DIR} ${PROJECT_SOURCE_DIR_LOCATION})
  set_target_properties(
    ${filename_we}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG
               ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/debug
               RUNTIME_OUTPUT_DIRECTORY_RELEASE
               ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

  # Set up the test
  add_test(NAME ${filename_we} COMMAND ${filename_we})
  set_tests_properties(${filename_we} PROPERTIES LABELS "quick")
endforeach()
