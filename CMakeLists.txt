cmake_minimum_required(VERSION 3.5)

project(Brainiac VERSION 0.1 LANGUAGES CXX)

add_subdirectory(tests)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Concurrent Core Gui Quick Quick3D Widgets)

qt_add_executable(Brainiac
    src/core/main.cpp
    src/gui/mainwindow.cpp
    src/gui/mainwindow.h
    src/core/agent.h src/core/agent.cpp
    src/core/agentinstance.h src/core/agentinstance.cpp
    src/core/agentreaderwriter.h src/core/agentreaderwriter.cpp
    src/core/brain/brain.h src/core/brain/brain.cpp
    src/core/scene.h src/core/scene.cpp
    src/core/simulation.h src/core/simulation.cpp
    src/core/brain/fuzzybase.h src/core/brain/fuzzybase.cpp
    src/core/brain/fuzzyoutput.h src/core/brain/fuzzyoutput.cpp
    src/core/brain/fuzzyinput.h src/core/brain/fuzzyinput.cpp
    src/core/brain/agentinstancebrain.h src/core/brain/agentinstancebrain.cpp
    src/core/brain/noise.h src/core/brain/noise.cpp
    src/core/brainiacglobals.h src/core/brainiacglobals.cpp
    src/core/channel.h src/core/channel.cpp
    src/core/generator/generatorbase.h src/core/generator/generatorbase.cpp
    src/core/generator/generatormanual.h src/core/generator/generatormanual.cpp
        src/core/generator/generatorpoint.h src/core/generator/generatorpoint.cpp
    src/core/generator/locator.h src/core/generator/locator.cpp
    src/gui/agent/agentinstacegeometryquick3d.h src/gui/agent/agentinstacegeometryquick3d.cpp
    src/gui/editorbase.h src/gui/editorbase.cpp
    src/gui/agent/brain/braineditor.h src/gui/agent/brain/braineditor.cpp
    src/gui/editoritem.h src/gui/editoritem.cpp
    src/gui/agent/brain/braineditoritem.h src/gui/agent/brain/braineditoritem.cpp
    src/gui/agent/body/bodyeditoritem.h src/gui/agent/body/bodyeditoritem.cpp
    src/gui/editoritemconnector.h src/gui/editoritemconnector.cpp
    src/core/brain/fuzzyand.cpp
    src/core/brain/fuzzyand.h
    src/core/brain/fuzzyor.cpp
    src/core/brain/fuzzyor.h
    src/core/body/body.cpp
    src/core/body/body.h
    src/core/body/bone.h src/core/body/bone.cpp
    src/core/body/bonebox.h src/core/body/bonebox.cpp
    src/core/body/agentinstancebody.h src/core/body/agentinstancebody.cpp
    src/core/body/agentinstancebone.h src/core/body/agentinstancebone.cpp
    src/core/body/agentinstancebonebox.h src/core/body/agentinstancebonebox.cpp
        src/core/brainiaclogger.cpp
        src/core/brainiaclogger.h

)

qt_add_resources(Brainiac "images"
    BASE "src/"
    FILES src/gui/pics/editor_logo_noise.png src/gui/pics/editor_logo_output.png
        src/gui/pics/editor_logo_input.png src/gui/pics/editor_logo_and.png
        src/gui/pics/editor_logo_or.png
)

qt_add_resources(Brainiac "qml"
        BASE "src/"
        FILES
        src/gui/AgentInstance.qml
)

set_target_properties(Brainiac PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)


target_link_libraries(Brainiac PUBLIC
        Qt::Widgets
        Qt6::Concurrent
        Qt::Core
        Qt::Gui
        Qt::Quick
        Qt::Quick3D
)

include_directories(
    src/gui/agent
)

qt_add_qml_module(Brainiac
    URI "BrainiacViewer"
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml"
    QML_FILES
        src/gui/MainViewer.qml
        RESOURCES
        src/gui/agent/shader/agentGeometry.vert
    IMPORTS
        QtQuick3D
)



include(GNUInstallDirs)
install(TARGETS Brainiac
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


