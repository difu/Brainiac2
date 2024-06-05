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
#include "brain/fuzzyoutput.h"
#include "brain/fuzzyand.h"
#include "brainiacglobals.h"
#include "generator/generatormanual.h"
#include "generator/locator.h"
#include "scene.h"
#include "simulation.h"
#include "brain/fuzzyor.h"
#include "src/gui/mainwindow.h"

int main(int argc, char *argv[]) {
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

    auto *newAnd = agent->brain()->addAndNode();
    newAnd->setName("And 1");
    newAnd->setEditorPos(400, 75);

    auto *newOr = agent->brain()->addOrNode();
    newOr->setName("Or 1");
    newOr->setEditorPos(400, 175);

    FuzzyOutput *newOutput = agent->brain()->addOutputNode();
    newOutput->setChannelId(BrainiacGlobals::CO_COLOR);
    newOutput->setName("Output 1 (color)");
    newOutput->setEditorPos(400, 225);
    auto *newNoise3 = agent->brain()->addNoiseNode();
    newNoise3->setRate(1.0);
    newNoise3->setName("Color Noise");
    newNoise3->setEditorPos(100, 225);

    FuzzyBase::connectFuzzies(newNoise, newAnd);
    FuzzyBase::connectFuzzies(newNoise2, newAnd);
    FuzzyBase::connectFuzzies(newNoise, newOr);
    FuzzyBase::connectFuzzies(newNoise2, newOr);
    FuzzyBase::connectFuzzies(newNoise3, newOutput);

    // End Brain

    const int numberOfLocators = 15;
    for (int i = 0; i < numberOfLocators; i++) {
        Locator *loc = gen->addLocator(agent);
        loc->setLocation(QVector3D(50 - cos(i) * 200, 0, 50 + sin(i) * 200));
        loc->setRotation(QVector3D(0, 50 + i * 10, 0));
        loc->setSeed((i + 1) * 4);
    }
    gen->apply();

    foreach(auto loc, gen->locators()) {
        float i = 0.0;
        loc->agentInstance()->outputChannels().value(BrainiacGlobals::CO_TZ)->setValue(4 + i / numberOfLocators);
        loc->agentInstance()->outputChannels().value(BrainiacGlobals::CO_RY)->setValue(1);
        i += 1.0;
    }

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
