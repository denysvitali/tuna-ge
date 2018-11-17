cmake_minimum_required(VERSION 3.12)
project(tuna_ge)

set(CMAKE_CXX_STANDARD 14)
set(GIT_EXECUTABLE git)

# the commit's SHA1, and whether the building workspace was dirty or not
execute_process(COMMAND
        "${GIT_EXECUTABLE}" describe --match=NeVeRmAtCh --always --abbrev=7 --dirty
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_SHA1
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# the date of the commit
execute_process(COMMAND
        "${GIT_EXECUTABLE}" log -1 --format=%ad --date=local
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_DATE
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# the subject of the commit
execute_process(COMMAND
        "${GIT_EXECUTABLE}" log -1 --format=%s
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_COMMIT_SUBJECT
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)


# generate version.cc
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/version.cc.in" "${CMAKE_CURRENT_BINARY_DIR}/version.cc" @ONLY)
list(APPEND SOURCES "${CMAKE_CURRENT_BINARY_DIR}/version.cc" version.hh)

find_package(glm REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)


add_library(tuna_ge STATIC tuna-ge.cpp RGBColor.h RGBColor.cpp Point.cpp Point.h)
include_directories( tuna_ge ${GLM_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
target_link_libraries(tuna_ge ${GLM_LIBRARY} ${GLUT_LIBRARY} ${OPENGL_LIBRARY} )
target_sources(tuna_ge PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/version.hh ${CMAKE_CURRENT_BINARY_DIR}/version.cc)

target_include_directories (tuna_ge PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})