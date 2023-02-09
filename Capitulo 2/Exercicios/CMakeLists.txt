cmake_minimum_required(VERSION 3.0.0)
project(Exemplo2.6.1 VERSION 0.1.0)

include(CTest)
enable_testing()
add_library(BigInt BigInt.cpp)
add_executable(Exercicio1 Exercicio1.cpp)
target_link_libraries(Exercicio1 PUBLIC BigInt)

                          target_include_directories(Exercicio1 PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
