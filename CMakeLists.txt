cmake_minimum_required(VERSION 3.18.0)
project(myProject)

# Location of additional cmake modules 
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

# c++ standard
set(CMAKE_CXX_STANDARD                      20)
set(CMAKE_CXX_STANDARD_REQUIRED             ON)

## google
#set(GOOGLETEST_DIR                          "extern/googletest")
#add_subdirectory(${GOOGLETEST_DIR})
## spdlog
#set(SPDLOG_DIR                              "extern/spdlog")
#add_subdirectory(${SPDLOG_DIR})
## toml++
#set(TOMLPLUSPLUS_DIR                        "extern/tomlplusplus")
#add_subdirectory(${TOMLPLUSPLUS_DIR})

add_subdirectory(src)
add_subdirectory(doc)

# ctags : make ctags
add_custom_target(ctags
    COMMENT "Generate tags file with ctags"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q ${CMAKE_SOURCE_DIR}
    COMMAND cp -f tags ${CMAKE_SOURCE_DIR}/src
)
