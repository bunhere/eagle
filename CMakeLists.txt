CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Eagle CXX)

SET(EVA_SOURCES
    src/main.cpp
)

INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs REQUIRED
    eina ecore evas ecore-evas
    ewebkit2
)

FOREACH(flag ${pkgs_CFLAGS})
    SET(EXTAR_CFLAGS "${EXTAR_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTAR_CFLAGS}")

ADD_EXECUTABLE(eagle ${EVA_SOURCES})
TARGET_LINK_LIBRARIES(eagle ${pkgs_LDFLAGS})
