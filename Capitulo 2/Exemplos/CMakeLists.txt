cmake_minimum_required(VERSION 3.0.0)
project(Exemplo2.6.1 VERSION 0.1.0)

include(CTest)
enable_testing()
add_library(BigInt BigInt.cpp)
add_executable(Exemplo2.6.1 Exemplo2.6.1.cpp)
target_link_libraries(Exemplo2.6.1 PUBLIC BigInt)

                          target_include_directories(Exemplo2.6.1 PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
