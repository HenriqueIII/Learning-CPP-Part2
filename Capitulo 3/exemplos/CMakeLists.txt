cmake_minimum_required(VERSION 3.0.0)
project(Exemplo10 VERSION 0.1.0)

include(CTest)
enable_testing()
add_library(Str1 Str1.cpp)
add_executable(Exemplo10 Exemplo10.cpp)
target_link_libraries(Exemplo10 PUBLIC Str1)

                          target_include_directories(Exemplo10 PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
