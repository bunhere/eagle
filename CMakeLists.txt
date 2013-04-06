cmake_minimum_required(VERSION 2.8)
project(Eagle CXX)

add_definitions(-DPROJECT_NAME=\"Eagle\")
add_definitions(-DTHEME_DIR=\"${CMAKE_BINARY_DIR}/theme\")

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(THIRDPARTY_DIR "${CMAKE_SOURCE_DIR}/src/3rdParty")

set(LIB_INSTALL_DIR "lib")
set(EXEC_INSTALL_DIR "bin")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${LIB_INSTALL_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${LIB_INSTALL_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${EXEC_INSTALL_DIR})

# features
set(ENABLE_LOG ON)

if (ENABLE_LOG)
    add_definitions(-DENABLE_LOG=1)
else ()
    add_definitions(-DENABLE_LOG=0)
endif ()

if (USE_MOBILE_PROFILE)
    # Now, this is only used for default resolution
    add_definitions(-DUSE_MOBILE_PROFILE)
endif ()

# Check package dependency.
set(PKG_list
    eina ecore evas ecore-evas ecore-x elementary
    sqlite3
)

if (USE_WEBKIT)
    add_definitions(-DUSE_WEBKIT=1)
    set(WEBKIT_PKG_NAME ewebkit)
    list(APPEND PKG_list ${WEBKIT_PKG_NAME})
elseif (USE_ELM_WEB)
    #elementary have webkit/efl dependency
    add_definitions(-DUSE_ELM_WEB=1)
else ()
    set(USE_WEBKIT2 On)
    add_definitions(-DUSE_WEBKIT2=1)
    set(WEBKIT_PKG_NAME ewebkit2)
    list(APPEND PKG_list ${WEBKIT_PKG_NAME})
endif ()

include(FindPkgConfig)
pkg_check_modules(PKGS REQUIRED ${PKG_list})

foreach(flag ${PKGS_CFLAGS})
    set(EAGLE_CFLAGS "${EAGLE_CFLAGS} ${flag}")
endforeach(flag)

set(EAGLE_LDFLAGS ${PKGS_LDFLAGS})

if (USE_WEBKIT OR USE_WEBKIT2)
    find_program(PKG_CONFIG_EXECUTABLE NAMES pkg-config DOC "pkg-config executable")
    execute_process(
            COMMAND ${PKG_CONFIG_EXECUTABLE} "--variable=datadir" ewebkit
            OUTPUT_VARIABLE EWEBKIT_DATA
            RESULT_VARIABLE _pkgconfig_failed)
    string(REGEX REPLACE "[\r\n]" "" EWEBKIT_DATA "${EWEBKIT_DATA}")
    message("[${EWEBKIT_DATA}]")
    add_definitions("-DWEBKIT_DEFAULT_THEME_PATH=\"${EWEBKIT_DATA}\"")
endif ()


# Set sources
set(EAGLE_SOURCES
    ${SOURCE_DIR}/Browser/Features/AutoFormFill.cpp
    ${SOURCE_DIR}/Browser/Features/Features.cpp

    ${SOURCE_DIR}/Browser/Browser.cpp
    ${SOURCE_DIR}/Browser/BrowserContent.cpp
    ${SOURCE_DIR}/Browser/FocusController.cpp
    ${SOURCE_DIR}/Browser/Urlbar.cpp
    ${SOURCE_DIR}/Browser/MultitabBar.cpp
    ${SOURCE_DIR}/Browser/ShortCut.cpp
    ${SOURCE_DIR}/Browser/Tab.cpp
    ${SOURCE_DIR}/Browser/WebView.cpp

    ${SOURCE_DIR}/EflWrappers/Object.cpp
    ${SOURCE_DIR}/EflWrappers/Window.cpp

    ${SOURCE_DIR}/ERU/ERUUrl.cpp
    ${SOURCE_DIR}/ERU/Logger.cpp
)

if (USE_WEBKIT)
    list(APPEND EAGLE_SOURCES
        ${SOURCE_DIR}/Browser/ewk/WebViewEWK.cpp
    )
elseif (USE_ELM_WEB)
    list(APPEND EAGLE_SOURCES
        ${SOURCE_DIR}/Browser/elm/WebViewElm.cpp
    )
else ()
    list(APPEND EAGLE_SOURCES
        ${SOURCE_DIR}/Browser/ewk2/WebViewEWK2.cpp
        ${SOURCE_DIR}/Browser/PopupMenu.cpp
    )
endif ()

set(EAGLE_EXECUTABLE_SOURCES
    ${EAGLE_SOURCES}
    ${SOURCE_DIR}/Main.cpp
)

include(theme/CMakeLists.txt)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ${EXTRA_CFLAGS} ${EAGLE_CFLAGS}")

add_executable(eagle ${EAGLE_EXECUTABLE_SOURCES})
target_link_libraries(eagle ${EAGLE_LDFLAGS})
include_directories(eagle
    "src/"
    "theme/"
)

install(TARGETS eagle DESTINATION "bin")

add_subdirectory(src/3rdParty)
add_subdirectory(src/tests)

