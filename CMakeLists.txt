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
#                                 ISPC setup                                     #
# ------------------------------------------------------------------------------ #

set(ISPC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ispc)
set(ISPC_BUILD_DIR ${PROJECT_BINARY_DIR}/include_ispc)

file(MAKE_DIRECTORY ${ISPC_BUILD_DIR})

set(ISPC_FILE "test")
set(ISPC_SOURCE ${ISPC_FILE}.ispc)
set(ISPC_HEADER ${ISPC_FILE}.h)
set(ISPC_OBJECT ${ISPC_FILE}.o)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ISPC_COMPILE_OPTIONS "-O0" )
elseif ()
    set(ISPC_COMPILE_OPTIONS "-O2" )
endif ()

if (APPLE OR UNIX)
    add_custom_command(
            OUTPUT ${PROJECT_BINARY_DIR}/${ISPC_OBJECT} ${PROJECT_BINARY_DIR}/include/${ISPC_HEADER}
            COMMAND ispc ${CMAKE_CURRENT_SOURCE_DIR}/${ISPC_SOURCE} --header-outfile=${PROJECT_BINARY_DIR}/include/${ISPC_HEADER} -o ${PROJECT_BINARY_DIR}/${ISPC_OBJECT} ${ISPC_COMPILE_OPTIONS}
            DEPENDS ${ISPC_SOURCE}
    )
endif (APPLE OR UNIX)

# Make a library from the results of the ispc compilation
add_library(${ISPC_LIB} STATIC
            ${ISPC_OBJECT})

# Set the linker language to C so the linker knows how to link
SET_TARGET_PROPERTIES(
        ${ISPC_LIB}
        PROPERTIES
        LINKER_LANGUAGE C
)

# ------------------------------------------------------------------------------ #
#                           Define exec and link                                 #
# ------------------------------------------------------------------------------ #

add_executable(${EXEC_NAME} main.cpp)
add_dependencies(${EXEC_NAME} ${ISPC_LIB})
target_include_directories(${EXEC_NAME} PRIVATE ${PROJECT_BINARY_DIR}/include)
target_link_libraries(${EXEC_NAME} ${ISPC_LIB})