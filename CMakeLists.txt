CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Eagle CXX)

ADD_DEFINITIONS(-DPROJECT_NAME=\"Eagle\")

SET(EAGLE_SOURCES
    src/browser/Browser.cpp
    src/browser/WebView.cpp

    src/EflWrappers/Object.cpp
    src/EflWrappers/Window.cpp

    src/Main.cpp
)

INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs REQUIRED
    eina ecore evas ecore-evas elementary
    ewebkit2
)

FOREACH(flag ${pkgs_CFLAGS})
    SET(EXTAR_CFLAGS "${EXTAR_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTAR_CFLAGS}")

ADD_EXECUTABLE(eagle ${EAGLE_SOURCES})
TARGET_LINK_LIBRARIES(eagle ${pkgs_LDFLAGS})
INCLUDE_DIRECTORIES(eagle "src/")
