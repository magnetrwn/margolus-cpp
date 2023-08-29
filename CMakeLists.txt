cmake_minimum_required(VERSION 3.12.0)
project(margolus)

file(GLOB PROJECT_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/popl/*.hpp"
)

if(MSVC)
    add_compile_options(/w4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(${PROJECT_NAME} ${PROJECT_FILES})

add_compile_definitions(-DMAX_SCREEN_ANY=16384)
add_compile_definitions(-DDEFAULT_NEW_WIDTH=80)
add_compile_definitions(-DDEFAULT_NEW_HEIGHT=48)
add_compile_definitions(-DDEFAULT_RENDERER=\"ANSI\")
add_compile_definitions(-DDEFAULT_NEW_RULESET=\"BILLIARD_BALL\")

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC include
    PUBLIC lib/popl
)
target_link_libraries(
    ${PROJECT_NAME}
)
target_compile_features(
    ${PROJECT_NAME}
    PRIVATE cxx_std_11
)
