include(CheckLanguage)

check_language(CXX)
if(NOT CMAKE_CXX_COMPILER)
    message(STATUS "NUNLauncher desktop: no C++ compiler found; skipping optional GUI targets")
    return()
endif()

enable_language(CXX)
find_package(Qt6 6.4 QUIET COMPONENTS Widgets)
if(NOT Qt6_FOUND)
    message(STATUS "NUNLauncher desktop: Qt 6 Widgets not found; skipping optional GUI targets")
    return()
endif()

add_executable(NUNLauncher.Desktop main.cpp)
set_target_properties(NUNLauncher.Desktop PROPERTIES
    OUTPUT_NAME "NUNWorkbench"
    CXX_EXTENSIONS OFF
)
target_compile_features(NUNLauncher.Desktop PRIVATE cxx_std_17)
target_compile_definitions(NUNLauncher.Desktop PRIVATE
    NUNLAUNCHER_VERSION="${PROJECT_VERSION}"
    NUNLAUNCHER_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)
target_link_libraries(NUNLauncher.Desktop PRIVATE Qt6::Widgets)

add_executable(NUNLauncher.AutoImprove auto_improve.cpp)
set_target_properties(NUNLauncher.AutoImprove PROPERTIES
    OUTPUT_NAME "NUNAutoImprove"
    CXX_EXTENSIONS OFF
)
target_compile_features(NUNLauncher.AutoImprove PRIVATE cxx_std_17)
target_link_libraries(NUNLauncher.AutoImprove PRIVATE Qt6::Widgets)

if(MSVC)
    target_compile_options(NUNLauncher.Desktop PRIVATE /W4 /utf-8)
    target_compile_options(NUNLauncher.AutoImprove PRIVATE /W4 /utf-8)
else()
    target_compile_options(NUNLauncher.Desktop PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
    target_compile_options(NUNLauncher.AutoImprove PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

if(BUILD_TESTING)
    add_test(NAME nunlauncher_desktop_version COMMAND NUNLauncher.Desktop --version)
    set_tests_properties(nunlauncher_desktop_version PROPERTIES
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
    )
endif()

install(TARGETS NUNLauncher.Desktop NUNLauncher.AutoImprove
    RUNTIME DESTINATION bin BUNDLE DESTINATION .)
