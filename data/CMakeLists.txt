cmake_minimum_required(VERSION 3.0.0)
project(data VERSION 0.1.0 LANGUAGES C)

add_library(${PROJECT_NAME} r2_r_example_functions.c)
add_library(sub::data ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE m)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/.
)