# gtest
set(GTEST_DIR "${THIRDPARTY_DIR}/gtest")

include_directories(
    ${GTEST_DIR}/include
    ${GTEST_DIR}
)

add_library(gtest SHARED
    ${GTEST_DIR}/src/gtest.cc
    ${GTEST_DIR}/src/gtest-death-test.cc
    ${GTEST_DIR}/src/gtest-filepath.cc
    ${GTEST_DIR}/src/gtest_main.cc
    ${GTEST_DIR}/src/gtest-port.cc
    ${GTEST_DIR}/src/gtest-test-part.cc
    ${GTEST_DIR}/src/gtest-typed-test.cc
)

add_definitions(-DGTEST_CREATE_SHARED_LIBRARY=1)
