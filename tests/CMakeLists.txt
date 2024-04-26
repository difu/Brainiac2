cmake_minimum_required(VERSION 3.5)

project(brainiacTest LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)
find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS Widgets)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    agent tst_agent.cpp
    ../src/core/agent.h ../src/core/agent.cpp
    ../src/core/agentinstance.h ../src/core/agentinstance.cpp
    ../src/core/brain/brain.h ../src/core/brain/brain.cpp
    ../src/core/scene.h ../src/core/scene.cpp
    ../src/core/simulation.h ../src/core/simulation.cpp
    ../src/core/brain/fuzzybase.h ../src/core/brain/fuzzybase.cpp
    ../src/core/brain/fuzzyoutput.h ../src/core/brain/fuzzyoutput.cpp
    ../src/core/channel.h ../src/core/channel.cpp
)

add_executable(
    simulation tst_simulation.cpp
    ../src/core/agent.h ../src/core/agent.cpp
    ../src/core/agentinstance.h ../src/core/agentinstance.cpp
    ../src/core/brain/brain.h ../src/core/brain/brain.cpp
    ../src/core/scene.h ../src/core/scene.cpp
    ../src/core/simulation.h ../src/core/simulation.cpp
    ../src/core/brain/fuzzybase.h ../src/core/brain/fuzzybase.cpp
    ../src/core/brain/fuzzyoutput.h ../src/core/brain/fuzzyoutput.cpp
    ../src/core/channel.h ../src/core/channel.cpp
)

add_test(NAME agent COMMAND agent)
add_test(NAME simulation COMMAND simulation)


target_link_libraries(agent PRIVATE Qt6::Gui)
target_link_libraries(agent PRIVATE Qt6::Widgets)
target_link_libraries(agent PRIVATE Qt${QT_VERSION_MAJOR}::Test)

target_link_libraries(simulation PRIVATE Qt6::Gui)
target_link_libraries(simulation PRIVATE Qt6::Widgets)
target_link_libraries(simulation PRIVATE Qt${QT_VERSION_MAJOR}::Test)
