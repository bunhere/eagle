CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Eagle CXX)

ADD_DEFINITIONS(-DPROJECT_NAME=\"Eagle\")
ADD_DEFINITIONS(-DTHEME_DIR=\"${CMAKE_BINARY_DIR}/theme\")

SET(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
SET(THIRDPARTY_DIR "${CMAKE_SOURCE_DIR}/src/3rdParty")

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
    ${SOURCE_DIR}/Browser/Features/AutoFormFill.cpp
    ${SOURCE_DIR}/Browser/Features/Features.cpp

    ${SOURCE_DIR}/Browser/Browser.cpp
    ${SOURCE_DIR}/Browser/FocusController.cpp
    ${SOURCE_DIR}/Browser/Urlbar.cpp
    ${SOURCE_DIR}/Browser/MultitabBar.cpp
    ${SOURCE_DIR}/Browser/ShortCut.cpp
    ${SOURCE_DIR}/Browser/Tab.cpp
    ${SOURCE_DIR}/Browser/WebView.cpp

    ${SOURCE_DIR}/EflWrappers/Object.cpp
    ${SOURCE_DIR}/EflWrappers/Window.cpp

    ${SOURCE_DIR}/ERU/ERUUrl.cpp
)

IF (USE_WEBKIT)
    LIST(APPEND EAGLE_SOURCES
        ${SOURCE_DIR}/Browser/ewk/WebViewEWK.cpp
    )
ELSEIF (USE_ELM_WEB)
ELSE ()
    LIST(APPEND EAGLE_SOURCES
        ${SOURCE_DIR}/Browser/ewk2/WebViewEWK2.cpp
        ${SOURCE_DIR}/Browser/PopupMenu.cpp
    )
ENDIF ()

SET(EAGLE_EXECUTABLE_SOURCES
    ${EAGLE_SOURCES}
    ${SOURCE_DIR}/Main.cpp
)

INCLUDE(theme/CMakeLists.txt)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ${EXTRA_CFLAGS} ${EAGLE_CFLAGS}")

ADD_EXECUTABLE(eagle ${EAGLE_EXECUTABLE_SOURCES})
TARGET_LINK_LIBRARIES(eagle ${EAGLE_LDFLAGS})
INCLUDE_DIRECTORIES(eagle
    "src/"
    "theme/"
)

INSTALL (TARGETS eagle DESTINATION "bin")

ADD_SUBDIRECTORY(src/3rdParty)
ADD_SUBDIRECTORY(src/tests)

