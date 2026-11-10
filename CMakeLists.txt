cmake_minimum_required(VERSION 3.16)
project(NUNLauncher VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(nunlauncher_boot_contract STATIC src/boot.c)
target_include_directories(nunlauncher_boot_contract PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(CTest)
if(BUILD_TESTING)
    add_executable(nunlauncher_boot_tests tests/test_boot.c)
    target_link_libraries(nunlauncher_boot_tests PRIVATE nunlauncher_boot_contract)
    add_test(NAME nunlauncher_boot_tests COMMAND nunlauncher_boot_tests)
endif()
