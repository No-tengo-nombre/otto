message(STATUS "Looking for cppcheck")
find_program(CPPCHECK_BIN NAMES cppcheck)

if(CPPCHECK_BIN)
  message(STATUS "Looking for cppcheck - done")
  list(
        APPEND CMAKE_CXX_CPPCHECK
            "${CPPCHECK_BIN}"
            "--enable=all"
            "--enable=warning,performance,portability,information"
            "--inconclusive"
            "--check-config"
            "--force"
            "--inline-suppr"
            "--suppressions-list=${CMAKE_SOURCE_DIR}/cppcheck_suppressions.txt"
            "--xml"
            "--output-file=${CMAKE_BINARY_DIR}/cppcheck.xml"
    )
else()
  message(STATUS "Looking for cppcheck - failed")
endif()
