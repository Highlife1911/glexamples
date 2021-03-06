
set(target Viewer-painters)
message(STATUS "Example ${target}")


# External libraries
# Qt5

# good resource: http://www.kdab.com/using-cmake-with-qt-5/

# http://qt-project.org/forums/viewthread/30006/
if(MSVC)
    cmake_policy(SET CMP0020 NEW)
endif()

find_package(Qt5Core    5.1 REQUIRED)
find_package(Qt5Gui     5.1 REQUIRED)
find_package(Qt5Widgets 5.1 REQUIRED)
find_package(Qt5OpenGL  5.1 REQUIRED)

if (NOT Qt5Core_FOUND)
    message("App ${target} skipped: Qt5 not found")
    return()
endif()

# Qt5 options

set(CMAKE_AUTOMOC ON)

# Includes

include_directories(
    BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GLRAW_INCLUDE_DIR}
)


# Libraries

set(libs
    ${GLEXAMPLES_DEPENDENCY_LIBRARIES}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::OpenGL
)


# Compiler definitions



# for compatibility between glm 0.9.4 and 0.9.5
add_definitions("-DGLM_FORCE_RADIANS")


# Sources

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/")

set(sources
    ${source_path}/viewer/Viewer.cpp
    ${source_path}/viewer/ViewerOptions.cpp

    ${source_path}/dithering/Dithering.cpp
    ${source_path}/dithering/DitheringOptions.cpp

    ${source_path}/InputHandling.cpp
    ${source_path}/plugin.cpp
)

set(api_includes
    ${include_path}/viewer/Viewer.h
    ${include_path}/viewer/ViewerOptions.h

    ${include_path}/dithering/Dithering.h
    ${include_path}/dithering/DitheringOptions.h

    ${include_path}/InputHandling.h
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$"
    ${header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$"
    ${source_group} ${sources})


# Build library

add_library(${target} SHARED ${api_includes} ${sources})

target_link_libraries(${target} ${libs})

target_compile_options(${target} PRIVATE ${DEFAULT_COMPILE_FLAGS})

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE              CXX
    FOLDER                      "${IDE_FOLDER}"
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH                ${include_path})


# Deployment

# Plugin library (release)
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_PLUGINS}       CONFIGURATIONS Release
    LIBRARY DESTINATION ${INSTALL_PLUGINS}       CONFIGURATIONS Release
    ARCHIVE DESTINATION ${INSTALL_LIB}
)

# Plugin library (debug)
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_PLUGINS_DEBUG} CONFIGURATIONS Debug
    LIBRARY DESTINATION ${INSTALL_PLUGINS_DEBUG} CONFIGURATIONS Debug
)
