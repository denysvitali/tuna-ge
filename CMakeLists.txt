cmake_minimum_required(VERSION 3.11)
project(tuna_ge)

set(CMAKE_CXX_STANDARD 17)
set(GIT_EXECUTABLE git)
set(CMAKE_CXX_COMPILER clang++)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
endif()

if(CI MATCHES 1)
    message("Running in CI mode")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCI=1")
    set(CI_FLAG CI=1)
    add_compile_options(-DCI=1)
endif()

# the commit's SHA1, and whether the building workspace was dirty or not
execute_process(COMMAND
        "${GIT_EXECUTABLE}" describe --match=NeVeRmAtCh --always --abbrev=7 --dirty
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_SHA1
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# the date of the commit
execute_process(COMMAND
        "${GIT_EXECUTABLE}" log -1 --format=%ad --date=local
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_DATE
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# the subject of the commit
execute_process(COMMAND
        "${GIT_EXECUTABLE}" log -1 --format=%s
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_COMMIT_SUBJECT
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)


# generate version.cc
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/version.cc.in" "${CMAKE_CURRENT_BINARY_DIR}/version.cc" @ONLY)
list(APPEND SOURCES "${CMAKE_CURRENT_BINARY_DIR}/version.cc" version.hh)


# Dependencies

find_package(glm REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

# look for freeimage
FIND_PATH(FREEIMAGE_INCLUDE_DIR FreeImage.h
        /usr/local/include
        /usr/include
        /opt/local/include
        )
FIND_LIBRARY(FREEIMAGE_LIBRARY freeimage
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        )

IF(FREEIMAGE_INCLUDE_DIR)
    IF(FREEIMAGE_LIBRARY)
        SET( FREEIMAGE_FOUND "YES" )
        SET( FREEIMAGE_LIBRARIES ${FREEIMAGE_LIBRARY} )
    ENDIF(FREEIMAGE_LIBRARY)
ENDIF(FREEIMAGE_INCLUDE_DIR)

message("FreeImage: ${STATUS_FREEIMG}")


# Testing Framework
enable_testing()
find_package(GTest REQUIRED)


find_program(MEMORYCHECK_COMMAND valgrind)
set( MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full" )

find_program(VALGRIND "valgrind")


add_library(tuna_ge STATIC tuna-ge.cpp
        structure/string/String.cpp
        structure/color/RGBColor.cpp
        structure/light/Light.cpp
        structure/mesh/Mesh.cpp
        structure/vertex/Vertex.cpp
        structure/texture/Texture.cpp
        structure/camera/Camera.cpp
        structure/material/Material.cpp
        structure/node/Node.cpp
        structure/object/Object.cpp
        structure/element/Element.cpp
        structure/list/List.cpp
        )
include_directories( tuna_ge ${GLM_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
target_link_libraries(tuna_ge ${GLM_LIBRARY} ${GLUT_LIBRARY} ${OPENGL_LIBRARY} )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/textures/ DESTINATION ${CMAKE_BINARY_DIR}/tuna-gui)

IF(FREEIMAGE_FOUND)
    INCLUDE_DIRECTORIES(${FREEIMAGE_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(tuna_ge ${FREEIMAGE_LIBRARY})
ENDIF(FREEIMAGE_FOUND)

add_executable(Test test/Main.cpp)
target_link_libraries(Test tuna_ge ${GTEST_BOTH_LIBRARIES})
add_test(Test Test)

if(VALGRIND)
    add_custom_target(valgrind-tunage-nodetest
            COMMAND "${VALGRIND}" --tool=memcheck --leak-check=full --show-reachable=yes --num-callers=20 --track-fds=yes $<TARGET_FILE:Test>)
endif()

target_sources(tuna_ge PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/version.hh ${CMAKE_CURRENT_BINARY_DIR}/version.cc)

target_include_directories (tuna_ge PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
