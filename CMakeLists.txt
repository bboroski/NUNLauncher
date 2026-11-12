cmake_minimum_required(VERSION 3.20)
project(NUNLauncher VERSION 0.2.0 LANGUAGES C)

include(CTest)

add_library(NUNLauncher.Contract STATIC src/boot_contract.c src/diagnostics.c)
target_include_directories(NUNLauncher.Contract PUBLIC include)
target_compile_features(NUNLauncher.Contract PUBLIC c_std_11)
if(MSVC)
    target_compile_options(NUNLauncher.Contract PRIVATE /W4 /utf-8)
else()
    target_compile_options(NUNLauncher.Contract PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

if(BUILD_TESTING)
    add_executable(NUNLauncher.ContractTests tests/test_boot_contract.c)
    target_link_libraries(NUNLauncher.ContractTests PRIVATE NUNLauncher.Contract)
    add_test(NAME nunlauncher_boot_contract COMMAND NUNLauncher.ContractTests)

    add_executable(NUNLauncher.DiagnosticsTests tests/test_diagnostics.c)
    target_link_libraries(NUNLauncher.DiagnosticsTests PRIVATE NUNLauncher.Contract)
    add_test(NAME nunlauncher_diagnostics COMMAND NUNLauncher.DiagnosticsTests)
endif()

option(NUNLAUNCHER_BUILD_DESKTOP "Build the optional NUN Workbench Qt desktop application" ON)
if(NUNLAUNCHER_BUILD_DESKTOP)
    add_subdirectory(desktop)
endif()
