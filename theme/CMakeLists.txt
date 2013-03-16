
find_program(EDJE_CC_EXECUTABLE edje_cc)
if (NOT EDJE_CC_EXECUTABLE)
    message(FATAL_ERROR "edje_cc could not be found")
endif ()

set(THEME_DIR "${CMAKE_BINARY_DIR}/theme")
file(MAKE_DIRECTORY ${THEME_DIR})

set(EAGLE_THEME ${THEME_DIR}/eagle.edj)
set(EAGLE_THEME_SOURCE ${CMAKE_SOURCE_DIR}/theme/eagle.edc)

add_custom_command(
    OUTPUT ${EAGLE_THEME}
    COMMAND ${EDJE_CC_EXECUTABLE} -v -id ${CMAKE_SOURCE_DIR}/theme ${EAGLE_THEME_DEFINITION} ${EAGLE_THEME_SOURCE} ${EAGLE_THEME}
    DEPENDS
        ${EAGLE_THEME_SOURCE}
        ${CMAKE_SOURCE_DIR}/theme/browser-view.edc
        ${CMAKE_SOURCE_DIR}/theme/browser-urlbar.edc
        ${CMAKE_SOURCE_DIR}/theme/multitab.edc
)

list(APPEND EAGLE_EXECUTABLE_SOURCES
    ${EAGLE_THEME}
)
