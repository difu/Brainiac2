cmake_minimum_required(VERSION 3.5)

project(brainiacTest LANGUAGES CXX)

enable_testing()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Quick3D Widgets Test)


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
    ../src/core/brainiacglobals.h ../src/core/brainiacglobals.cpp
    ../src/core/channel.h ../src/core/channel.cpp
    ../src/core/generator/generatorbase.h ../src/core/generator/generatorbase.cpp
    ../src/core/generator/generatormanual.h ../src/core/generator/generatormanual.cpp
    ../src/core/generator/locator.h ../src/core/generator/locator.cpp
)


include_directories(
    ../src/gui/agent
    ../
)

qt_add_qml_module(agent
    URI "BrainiacViewer"
    VERSION 1.0
    QML_FILES
        ../src/gui/MainViewer.qml
    IMPORTS
        QtQuick3D
    NO_RESOURCE_TARGET_PATH
    SOURCES ../src/gui/agent/agentinstacegeometryquick3d.h ../src/gui/agent/agentinstacegeometryquick3d.cpp
    QML_FILES ../src/gui/AgentInstance.qml
)

add_test(NAME agent COMMAND agent)

target_link_libraries(agent PUBLIC
    Qt::Widgets
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::Quick3D
    Qt::Test
)
# target_link_libraries(agent PRIVATE Qt${QT_VERSION_MAJOR}::Test)
