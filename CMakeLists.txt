CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Eagle CXX)

ADD_DEFINITIONS(-DPROJECT_NAME=\"Eagle\")

# Check package dependency.
SET(PKG_LIST
    eina ecore evas ecore-evas elementary
    sqlite3
)

IF (USE_WEBKIT)
    ADD_DEFINITIONS(-DUSE_WEBKIT=1)
    LIST(APPEND PKG_LIST ewebkit)
ELSEIF (USE_ELM_WEB)
    #elementary have webkit/efl dependency
    ADD_DEFINITIONS(-DUSE_ELM_WEB=1)
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
    src/Browser/Features/AutoFormFill.cpp
    src/Browser/Features/Features.cpp

    src/Browser/Browser.cpp
    src/Browser/Urlbar.cpp
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

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CFLAGS} ${EAGLE_CFLAGS}")

ADD_EXECUTABLE(eagle ${EAGLE_SOURCES})
TARGET_LINK_LIBRARIES(eagle ${EAGLE_LDFLAGS})
INCLUDE_DIRECTORIES(eagle
    "src/"
    "theme/"
)

INSTALL (TARGETS eagle DESTINATION "bin")
