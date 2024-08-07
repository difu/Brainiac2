cmake_minimum_required(VERSION 3.5)

project(brainiacTest LANGUAGES CXX)

enable_testing()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Quick3D Widgets Test)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
        ../src/gui/mainwindow.cpp
        ../src/gui/mainwindow.h
        ../src/core/agent.h ../src/core/agent.cpp
        ../src/core/agentinstance.h ../src/core/agentinstance.cpp
        ../src/core/agentreaderwriter.h ../src/core/agentreaderwriter.cpp
        ../src/core/brain/brain.h ../src/core/brain/brain.cpp
        ../src/core/scene.h ../src/core/scene.cpp
        ../src/core/scenereaderwriter.h ../src/core/scenereaderwriter.cpp
        ../src/core/simulation.h ../src/core/simulation.cpp
        ../src/core/brain/fuzzybase.h ../src/core/brain/fuzzybase.cpp
        ../src/core/brain/fuzzyoutput.h ../src/core/brain/fuzzyoutput.cpp
        ../src/core/brain/fuzzyinput.h ../src/core/brain/fuzzyinput.cpp
        ../src/core/brain/agentinstancebrain.h ../src/core/brain/agentinstancebrain.cpp
        ../src/core/brain/noise.h ../src/core/brain/noise.cpp
        ../src/core/brainiacglobals.h ../src/core/brainiacglobals.cpp
        ../src/core/channel.h ../src/core/channel.cpp
        ../src/core/generator/generatorbase.h ../src/core/generator/generatorbase.cpp
        ../src/core/generator/generatormanual.h ../src/core/generator/generatormanual.cpp
        ../src/core/generator/generatorpoint.h ../src/core/generator/generatorpoint.cpp
        ../src/core/generator/locator.h ../src/core/generator/locator.cpp
        ../src/gui/agent/agentinstacegeometryquick3d.h ../src/gui/agent/agentinstacegeometryquick3d.cpp
        ../src/gui/editorbase.h ../src/gui/editorbase.cpp
        ../src/gui/agent/brain/braineditor.h ../src/gui/agent/brain/braineditor.cpp
        ../src/gui/editoritem.h ../src/gui/editoritem.cpp
        ../src/gui/agent/brain/braineditoritem.h ../src/gui/agent/brain/braineditoritem.cpp
        ../src/gui/agent/body/bodyeditoritem.h ../src/gui/agent/body/bodyeditoritem.cpp
        ../src/gui/editoritemconnector.h ../src/gui/editoritemconnector.cpp
        ../src/core/brain/fuzzyand.cpp
        ../src/core/brain/fuzzyand.h
        ../src/core/brain/fuzzyor.cpp
        ../src/core/brain/fuzzyor.h
        ../src/core/body/body.cpp
        ../src/core/body/body.h
        ../src/core/body/bone.h ../src/core/body/bone.cpp
        ../src/core/body/bonebox.h ../src/core/body/bonebox.cpp
        ../src/core/body/agentinstancebody.h ../src/core/body/agentinstancebody.cpp
        ../src/core/body/agentinstancebone.h ../src/core/body/agentinstancebone.cpp
        ../src/core/body/agentinstancebonebox.h ../src/core/body/agentinstancebonebox.cpp
        ../src/gui/agent/agentinstacegeometryquick3d.h ../src/gui/agent/agentinstacegeometryquick3d.cpp
        ../src/core/brainiaclogger.cpp ../src/core/brainiaclogger.h
        ../src/core/basereaderwriter.h ../src/core/basereaderwriter.cpp
)

add_executable(
        test_agent tst_agent.cpp
        ${SOURCE_FILES}
)

add_executable(
        test_simulation tst_simulation.cpp
        ${SOURCE_FILES}
)

add_executable(
        test_scene tst_scene.cpp
        ${SOURCE_FILES}
)


include_directories(
    ../src/gui/agent
    ../
)

#qt_add_qml_module(test_agent
#    URI "BrainiacViewer"
#    VERSION 1.0
#    QML_FILES
#        ../src/gui/MainViewer.qml
#    IMPORTS
#        QtQuick3D
#    NO_RESOURCE_TARGET_PATH
#    QML_FILES ../src/gui/AgentInstance.qml
#)

add_test(NAME test_agent COMMAND test_agent)

target_link_libraries(test_agent PUBLIC
    Qt::Widgets
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::Quick3D
    Qt::Test
)

add_test(NAME test_simulation COMMAND test_simulation)

target_link_libraries(test_simulation PUBLIC
        Qt::Widgets
        Qt::Core
        Qt::Gui
        Qt::Quick
        Qt::Quick3D
        Qt::Test
)

add_test(NAME test_scene COMMAND test_scene)

target_link_libraries(test_scene PUBLIC
        Qt::Widgets
        Qt::Core
        Qt::Gui
        Qt::Quick
        Qt::Quick3D
        Qt::Test
)
# target_link_libraries(agent PRIVATE Qt${QT_VERSION_MAJOR}::Test)
