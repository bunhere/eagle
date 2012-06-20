CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Eagle CXX)

ADD_DEFINITIONS(-DPROJECT_NAME=\"Eagle\")

# Check package dependency.
SET(PKG_LIST
    eina ecore evas ecore-evas elementary
)

IF (USE_WEBKIT)
    ADD_DEFINITIONS(-DUSE_WEBKIT=1)
    LIST(APPEND PKG_LIST ewebkit)
ELSE ()
    ADD_DEFINITIONS(-DUSE_WEBKIT2=1)
    LIST(APPEND PKG_LIST ewebkit2)
ENDIF ()

INCLUDE(FindPkgConfig)
pkg_check_modules(PKGS REQUIRED ${PKG_LIST})

FOREACH(flag ${PKGS_CFLAGS})
    SET(EAGLE_CFLAGS "${EAGLE_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(EAGLE_LDFLAGS ${PKGS_LDFLAGS})

# Set sources
SET(EAGLE_SOURCES
    src/browser/Browser.cpp
    src/browser/Urlbar.cpp
    src/browser/WebView.cpp

    src/EflWrappers/Object.cpp
    src/EflWrappers/Window.cpp

    src/Main.cpp
)

IF (USE_WEBKIT)
    LIST(APPEND EAGLE_SOURCES
        src/browser/ewk/WebViewEWK.cpp
    )
ELSE ()
    LIST(APPEND EAGLE_SOURCES
        src/browser/ewk2/WebViewEWK2.cpp
    )
ENDIF ()

INCLUDE(theme/CMakeLists.txt)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CFLAGS} ${EAGLE_CFLAGS}")

ADD_EXECUTABLE(eagle ${EAGLE_SOURCES})
TARGET_LINK_LIBRARIES(eagle ${EAGLE_LDFLAGS})
INCLUDE_DIRECTORIES(eagle "src/")
