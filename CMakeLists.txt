CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Eagle CXX)

ADD_DEFINITIONS(-DPROJECT_NAME=\"Eagle\")
ADD_DEFINITIONS(-DTHEME_DIR=\"${CMAKE_BINARY_DIR}/theme\")

# Check package dependency.
SET(PKG_LIST
    eina ecore evas ecore-evas elementary
    sqlite3
)

IF (USE_WEBKIT)
    ADD_DEFINITIONS(-DUSE_WEBKIT=1)
    SET (WEBKIT_PKG_NAME ewebkit)
    LIST(APPEND PKG_LIST ${WEBKIT_PKG_NAME})
ELSEIF (USE_ELM_WEB)
    #elementary have webkit/efl dependency
    ADD_DEFINITIONS(-DUSE_ELM_WEB=1)
ELSE ()
    ADD_DEFINITIONS(-DUSE_WEBKIT2=1)
    SET (WEBKIT_PKG_NAME ewebkit2)
    LIST(APPEND PKG_LIST ${WEBKIT_PKG_NAME})
ENDIF ()

INCLUDE(FindPkgConfig)
pkg_check_modules(PKGS REQUIRED ${PKG_LIST})

FOREACH(flag ${PKGS_CFLAGS})
    SET(EAGLE_CFLAGS "${EAGLE_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(EAGLE_LDFLAGS ${PKGS_LDFLAGS})

IF (USE_WEBKIT OR USE_WEBKIT2)
    FIND_PROGRAM(PKG_CONFIG_EXECUTABLE NAMES pkg-config DOC "pkg-config executable")
    EXECUTE_PROCESS(
            COMMAND ${PKG_CONFIG_EXECUTABLE} "--variable=datadir" ewebkit
            OUTPUT_VARIABLE EWEBKIT_DATA
            RESULT_VARIABLE _pkgconfig_failed)
    string(REGEX REPLACE "[\r\n]" "" EWEBKIT_DATA "${EWEBKIT_DATA}")
    MESSAGE("[${EWEBKIT_DATA}]")
    ADD_DEFINITIONS("-DWEBKIT_DEFAULT_THEME_PATH=\"${EWEBKIT_DATA}\"")
ENDIF ()


# Set sources
SET(EAGLE_SOURCES
    src/Browser/Features/AutoFormFill.cpp
    src/Browser/Features/Features.cpp

    src/Browser/Browser.cpp
    src/Browser/Urlbar.cpp
    src/Browser/MultitabBar.cpp
    src/Browser/ShortCut.cpp
    src/Browser/Tab.cpp
    src/Browser/WebView.cpp

    src/EflWrappers/Object.cpp
    src/EflWrappers/Window.cpp

    src/Main.cpp
)

IF (USE_WEBKIT)
    LIST(APPEND EAGLE_SOURCES
        src/Browser/ewk/WebViewEWK.cpp
    )
ELSEIF (USE_ELM_WEB)
ELSE ()
    LIST(APPEND EAGLE_SOURCES
        src/Browser/ewk2/WebViewEWK2.cpp
        src/Browser/PopupMenu.cpp
    )
ENDIF ()

INCLUDE(theme/CMakeLists.txt)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ${EXTRA_CFLAGS} ${EAGLE_CFLAGS}")

ADD_EXECUTABLE(eagle ${EAGLE_SOURCES})
TARGET_LINK_LIBRARIES(eagle ${EAGLE_LDFLAGS})
INCLUDE_DIRECTORIES(eagle
    "src/"
    "theme/"
)

INSTALL (TARGETS eagle DESTINATION "bin")
