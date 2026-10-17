cmake_minimum_required(VERSION 3.16)
project(WaterNetworkOptimizer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(water_core
    src/flow_network.cpp
    src/dinic.cpp
    src/min_cost_flow.cpp
    src/network_builder.cpp
)
target_include_directories(water_core PUBLIC include)

add_executable(water_optimizer src/main.cpp)
target_link_libraries(water_optimizer PRIVATE water_core)

add_executable(water_optimizer_tests tests/test_main.cpp)
target_link_libraries(water_optimizer_tests PRIVATE water_core)
enable_testing()
add_test(NAME water_optimizer_tests COMMAND water_optimizer_tests)
