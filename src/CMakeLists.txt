cmake_minimum_required(VERSION 3.18.0)
project(pwl)
set(PROGRAM_NAME                            "pwl")

# dir
set(SRC_DIR                                 "${CMAKE_SOURCE_DIR}/src/")
set(TEST_SRC_DIR                            "${CMAKE_SOURCE_DIR}/src/")
set(INC_DIR                                 "${SRC_DIR}")
set(INST_DIR                                "${CMAKE_SOURCE_DIR}/install")
set(EXTERN_DIR                              "/media/PROJECT02/extern")
#set(EXTERN_DIR                              "../../../extern")

# git version
execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
configure_file(${CMAKE_SOURCE_DIR}/src/${PROGRAM_NAME}_git.hpp.in ${CMAKE_SOURCE_DIR}/src/${PROGRAM_NAME}_git.hpp)

# program name/build dir
set(RUNTIME_OUTPUT_DIRECTORY                "${CMAKE_SOURCE_DIR}/build/release")
set(ARCHIVE_OUTPUT_DIRECTORY                "${CMAKE_SOURCE_DIR}/build/release")
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message("-- Debug build.")
    set(RUNTIME_OUTPUT_DIRECTORY            "${CMAKE_SOURCE_DIR}/build/debug")
    set(ARCHIVE_OUTPUT_DIRECTORY            "${CMAKE_SOURCE_DIR}/build/debug")
    set(MY_EXE_NAME                         "${PROGRAM_NAME}.dbg.exe")
    set(MY_LIB_NAME                         "${PROGRAM_NAME}.dbg")
    set(MY_TEST_EXE_NAME                    "${PROGRAM_NAME}.dbg.test.exe")
    set(MY_TEST_LIB_NAME                    "${PROGRAM_NAME}.dbg.test")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message("-- Release build.")
    set(RUNTIME_OUTPUT_DIRECTORY            "${CMAKE_SOURCE_DIR}/build/release")
    set(ARCHIVE_OUTPUT_DIRECTORY            "${CMAKE_SOURCE_DIR}/build/release")
    set(MY_EXE_NAME                         "${PROGRAM_NAME}.exe")
    set(MY_LIB_NAME                         "${PROGRAM_NAME}")
    set(MY_TEST_EXE_NAME                    "${PROGRAM_NAME}.test.exe")
    set(MY_TEST_LIB_NAME                    "${PROGRAM_NAME}.test")
endif()

# sources/headers
set(MY_MAIN_SOURCE                                  "${SRC_DIR}/${PROGRAM_NAME}_main.cpp")
set(MY_MAIN_HEADER                                  "${SRC_DIR}/${PROGRAM_NAME}_main.hpp")
file(GLOB MY_SOURCES LIST_DIRECTORIES true          "${SRC_DIR}/${PROGRAM_NAME}_*.cpp")
file(GLOB MY_HEADERS LIST_DIRECTORIES true          "${SRC_DIR}/${PROGRAM_NAME}_*.hpp")

set(MY_TEST_MAIN_SOURCE                             "${SRC_DIR}/${PROGRAM_NAME}_main.test.cpp")
set(MY_TEST_MAIN_HEADER                             "${SRC_DIR}/${PROGRAM_NAME}_main.test.hpp")
file(GLOB MY_TEST_SOURCES LIST_DIRECTORIES true     "${SRC_DIR}/${PROGRAM_NAME}_*.test.cpp")
file(GLOB MY_TEST_HEADERS LIST_DIRECTORIES true     "${SRC_DIR}/${PROGRAM_NAME}_*.test.hpp")

list(REMOVE_ITEM MY_SOURCES "${MY_MAIN_SOURCE}" ${MY_TEST_SOURCES})
list(REMOVE_ITEM MY_HEADERS "${MY_MAIN_HEADER}" ${MY_TEST_HEADERS})
list(REMOVE_ITEM MY_TEST_SOURCES "${MY_TEST_MAIN_SOURCE}")
list(REMOVE_ITEM MY_TEST_HEADERS "${MY_TEST_MAIN_HEADER}")

# build : make -j
include_directories(${INC_DIR})
add_library(${MY_LIB_NAME} STATIC           ${MY_SOURCES})
add_executable(${MY_EXE_NAME}               ${MY_MAIN_SOURCE})
target_link_libraries(${MY_EXE_NAME}        ${MY_LIB_NAME})
set_target_properties(${MY_EXE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
set_target_properties(${MY_LIB_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})

# googletest : make & ./pwl.dbg.test.exe
include_directories("${EXTERN_DIR}/googletest/googletest/include")
set(GOOGLETEST_LIBRARIES                    "${EXTERN_DIR}/googletest/build/lib/libgtest.a")
add_executable(${MY_TEST_EXE_NAME}          ${MY_TEST_MAIN_SOURCE} ${MY_TEST_SOURCES})
target_link_libraries(${MY_TEST_EXE_NAME}   ${MY_LIB_NAME})
target_link_libraries(${MY_TEST_EXE_NAME}   ${GOOGLETEST_LIBRARIES})
set_target_properties(${MY_TEST_EXE_NAME}   PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
set_target_properties(${MY_TEST_EXE_NAME}   PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})

## openmp
#find_package(OpenMP)
#if(OPENMP_FOUND)
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#endif(OPENMP_FOUND)

# spdlog
set(SPDLOG_DIR                              "${EXTERN_DIR}/spdlog")
#set(SPDLOG_DIR                              "/media/PROJECT02/extern/spdlog")
set(SPDLOG_INC_DIR                          "${SPDLOG_DIR}/include")
set(SPDLOG_LIBRARIES                        "${SPDLOG_DIR}/build/libspdlog.a")
include_directories(${SPDLOG_INC_DIR})
target_link_libraries(${MY_EXE_NAME}        ${SPDLOG_LIBRARIES})
target_link_libraries(${MY_TEST_EXE_NAME}   ${SPDLOG_LIBRARIES})
##include_directories("../{SPDLOG_DIR}/include")
##target_link_libraries(${MY_EXE_NAME}        spdlog)
##find_package(spdlog)
##if(spdlog_FOUND)
##    target_link_libraries(${MY_EXE_NAME} spdlog)
##endif(spdlog_FOUND)

# gzstream
find_package(GZSTREAM)
if (GZSTREAM_FOUND)
    include_directories(${GZSTREAM_INCLUDE_DIRS})
    target_link_libraries(${MY_EXE_NAME} ${GZSTREAM_LIBRARIES})
    target_link_libraries(${MY_TEST_EXE_NAME} ${GZSTREAM_LIBRARIES})
    target_link_libraries(${MY_EXE_NAME} z)
    target_link_libraries(${MY_TEST_EXE_NAME} z)
endif(GZSTREAM_FOUND)

# fmt
find_package(fmt)
if(fmt_FOUND)
    #    include_directories(${fmt_INCLUDE_DIRS})
    target_link_libraries(${MY_EXE_NAME} fmt::fmt)
    target_link_libraries(${MY_TEST_EXE_NAME} fmt::fmt)
endif(fmt_FOUND)

## tcl/tk
#find_package(TCL)
#if(TCL_FOUND)
#    include_directories(${TCL_INCLUDE_PATH})
#    target_link_libraries(${MY_EXE_NAME} ${TCL_LIBRARY})
#endif(TCL_FOUND)

## tomlplusplus
#include_directories("../${TOMLPLUSPLUS_DIR}/include")

# install : make install
install(TARGETS ${MY_EXE_NAME} RUNTIME DESTINATION "${INST_DIR}/bin")
install(TARGETS ${MY_LIB_NAME} ARCHIVE DESTINATION "${INST_DIR}/lib")
install(FILES ${MY_HEADERS} DESTINATION "${INST_DIR}/include")
