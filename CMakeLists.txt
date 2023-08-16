cmake_minimum_required(VERSION 3.12.0)
project(cpp-margolus)

file(GLOB PROJECT_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

if(MSVC)
    add_compile_options(/w4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(${PROJECT_NAME} ${PROJECT_FILES})

add_compile_definitions(-DMAX_SCREEN_ANY=512)
add_compile_definitions(-DDEFAULT_WIDTH=36)
add_compile_definitions(-DDEFAULT_HEIGHT=14)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC include
)
target_link_libraries(
    ${PROJECT_NAME}
)
target_compile_features(
    ${PROJECT_NAME}
    PRIVATE cxx_std_11
)
