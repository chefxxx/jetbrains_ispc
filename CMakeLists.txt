# ------------------------------------------------------------------------------ #
#                                Project setup                                   #
# ------------------------------------------------------------------------------ #

cmake_minimum_required(VERSION 3.30)

set(PROJECT_NAME "newton_frac")
set(EXEC_NAME ${PROJECT_NAME})
set(ISPC_LIB ${PROJECT_NAME}_ispc_lib)

project(${PROJECT_NAME} LANGUAGES ISPC CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------ #
#                                Compile options                                 #
# ------------------------------------------------------------------------------ #

if (APPLE OR UNIX)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(ISPC_FLAGS -O0 --math-lib=default)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=undefined,address -O0 -Wall -Wextra -pedantic -Werror")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
    else()
        set(ISPC_FLAGS -O2 --math-lib=fast)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -funroll-loops")
    endif()
endif (APPLE OR UNIX)

# ------------------------------------------------------------------------------ #
#                                 ISPC setup                                     #
# ------------------------------------------------------------------------------ #

set(ISPC_BUILD_DIR ${PROJECT_BINARY_DIR}/include_ispc)
set(ISPC_OBJECTS "")
set(ISPC_HEADERS "")

file(MAKE_DIRECTORY ${ISPC_BUILD_DIR})
file(GLOB ISPC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/ispc/*.ispc")

foreach (ISPC_FILE ${ISPC_SOURCES})
    get_filename_component(ISPC_NAME ${ISPC_FILE} NAME_WE)
    set(ISPC_HEADER ${ISPC_NAME}.h)
    set(ISPC_OBJECT ${ISPC_NAME}.o)

    if (APPLE OR UNIX)
        add_custom_command(
                OUTPUT ${PROJECT_BINARY_DIR}/${ISPC_OBJECT} ${ISPC_BUILD_DIR}/${ISPC_HEADER}
                COMMAND ispc ${ISPC_FILE}
                        --header-outfile=${ISPC_BUILD_DIR}/${ISPC_HEADER}
                        -o ${PROJECT_BINARY_DIR}/${ISPC_OBJECT}
                        ${ISPC_FLAGS}
                DEPENDS ${ISPC_FILE}
        )
    endif (APPLE OR UNIX)

    list(APPEND ISPC_HEADERS ${ISPC_BUILD_DIR}/${ISPC_HEADER})
    list(APPEND ISPC_OBJECTS ${PROJECT_BINARY_DIR}/${ISPC_OBJECT})
endforeach ()

# ------------------------------------------------------------------------------ #
#                          Build static lib from ispc                            #
# ------------------------------------------------------------------------------ #

# Make a library from the results of the ispc compilation
add_library(${ISPC_LIB} STATIC
            ${ISPC_OBJECTS})

# Set the linker language to C so the linker knows how to link
SET_TARGET_PROPERTIES(
        ${ISPC_LIB}
        PROPERTIES
        LINKER_LANGUAGE C
)

target_include_directories(${ISPC_LIB} PUBLIC ${ISPC_BUILD_DIR})

# ------------------------------------------------------------------------------ #
#                           Define exec and link                                 #
# ------------------------------------------------------------------------------ #

add_executable(${EXEC_NAME} main.cpp tasksys.cpp)
add_dependencies(${EXEC_NAME} ${ISPC_LIB})
target_link_libraries(${EXEC_NAME} PUBLIC ${ISPC_LIB})
target_include_directories(${EXEC_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")