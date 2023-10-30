cmake_minimum_required(VERSION 3.0.0)
project(simulated_annealing VERSION 0.1.0 LANGUAGES C)

add_library(${PROJECT_NAME} sa.c sa_r2_r_basic.c)
add_library(sub::sa ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/.
)