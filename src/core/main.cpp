//#include "../gui/mainwindow.h"

#include <QApplication>
#include <QMetaObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QtQuick3D/qquick3d.h>
#include "agent.h"
#include "agentinstance.h"
#include "brain/brain.h"
#include "brain/noise.h"
#include "brainiacglobals.h"
#include "generator/generatormanual.h"
#include "generator/locator.h"
#include "scene.h"
#include "simulation.h"
#include "src/gui/mainwindow.h"

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{file}:%{line} %{function} -> %{if-category}%{category}: %{endif}%{message}");

    QApplication app(argc, argv);

    QSurfaceFormat::setDefaultFormat(QQuick3D::idealSurfaceFormat());

    QQmlApplicationEngine engine;
    QQmlContext context(&engine);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("BrainiacViewer", "MainViewer");

    //QWidget widget;
    //widget.show();

    Scene *scene = new Scene;
    MainWindow mainWindow(scene);
    mainWindow.setGeometry(100, 100, 800, 500);
    mainWindow.show();
    scene->setQQmlApplivationEngine(&engine);
    GeneratorManual *gen = new GeneratorManual(scene);
    Agent *agent = new Agent(scene);

    // Brain
    auto *newNoise = agent->brain()->addNoiseNode();
    newNoise->setRate(1.0);
    newNoise->setName("Noise 1");
    newNoise->setEditorPos(100, 100);
    auto *newNoise2 = agent->brain()->addNoiseNode();
    newNoise2->setRate(1.0);
    newNoise2->setName("Noise 2");
    newNoise2->setEditorPos(100, 150);


    FuzzyOutput *newOutput = agent->brain()->addOutputNode();
    newOutput->setChannelId(BrainiacGlobals::CO_TZ);
    newOutput->setName("Output 1 (tz)");

    // End Brain

    for (int i = 0; i < 15; i++) {
        Locator *loc = gen->addLocator(agent);
        loc->setLocation(QVector3D(50 + 100 * i, 0, 50 + 10 * i));
        loc->setRotation(QVector3D(0, 50 + i * 10, 0));
        loc->setSeed((i + 1) * 4);
    }
    gen->apply();

    AgentInstance *myInstance = scene->agentInstances().constFirst();
    myInstance->outputChannels().value(BrainiacGlobals::CO_TZ)->setValue(4);
    myInstance->outputChannels().value(BrainiacGlobals::CO_RY)->setValue(1);
    AgentInstance *myInstance2 = scene->agentInstances().constLast();
    myInstance2->outputChannels().value(BrainiacGlobals::CO_TZ)->setValue(3);
    myInstance2->outputChannels().value(BrainiacGlobals::CO_RY)->setValue(-1);
    agent->setDefaultAgentInstance(myInstance);

    mainWindow.setMainEditor(agent->brain()->brainEditor());

    scene->simulation()->setEndFrame(120);
    scene->simulation()->startSimulation();

    // Debug

    return app.exec();
}
