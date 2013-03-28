
set(TESTS_DIR "${SOURCE_DIR}/tests")

set(ERTESTS_BINARIES
    test_url
    test_window
)

include_directories("${THIRDPARTY_DIR}/gtest/include"
)

foreach (testName ${ERTESTS_BINARIES})
    add_executable(${testName} "${TESTS_DIR}/${testName}.cpp" ${EAGLE_SOURCES})
    target_link_libraries(${testName} ${EAGLE_LDFLAGS} gtest)
endforeach ()
