cmake_minimum_required(VERSION 3.0.0)
project(simulated_annealing_metaheuristics_lab_demos VERSION 0.1.0 LANGUAGES C)

#include(CTest)
#enable_testing()

# target_include_directories(${PROJECT_NAME} PUBLIC ../../sa ../../data testDir)
# target_include_directories(${PROJECT_NAME} PUBLIC testDir)
# add_subdirectory(testDir)


add_executable(simulated_annealing_metaheuristics_lab_demos main.c batch.c)

target_link_libraries(${PROJECT_NAME}
    sub::sa
    sub::data
)

#target_include_directories( ${PROJECT_NAME}
#    PUBLIC ${PROJECT_SOURCE_DIR}/..
#)


#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
