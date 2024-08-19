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
#include "body/body.h"
#include "brain/fuzzyor.h"
#include "generator/generatorpoint.h"
#include "src/gui/mainwindow.h"

int main(int argc, char *argv[]) {
    qSetMessagePattern("%{file}:%{line} %{type} %{function} -> %{if-category}%{category}: %{endif}%{message}");

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
    GeneratorManual *gen1 = new GeneratorManual(scene);
    GeneratorManual *gen2 = new GeneratorManual(scene);

    auto *agent = new Agent(scene);
    if (!agent->setName("Agent1_Body")) {
        qFatal("Agent with the same name exists");
    }
    auto *agent2 = new Agent(scene);
    if (!agent2->setName("Agent2_NoBody")) {
        qFatal("Agent with the same name exists");
    }

    auto *agent3 = new Agent(scene);
    if (!agent3->setName("Agent3_NoBody")) {
        qFatal("Agent with the same name exists");
    }

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
    // Body
    {
        BoneBox *rootBone = agent->body()->addBoneBox(1, 0, "root");
        BoneBox *leftBone = agent->body()->addBoneBox(2, 1, "left");
        BoneBox *rightBone = agent->body()->addBoneBox(3, 1, "right");
        rootBone->setTranslation(QVector3D(0, 0, 0));
        leftBone->setTranslation(QVector3D(30, -20, 0));
        rightBone->setTranslation(QVector3D(-30, -20, 0));
        // qDebug().noquote() << "Body QML :" << agent->body()->skeletonQML();
    }
    {
        BoneBox *rootBone = agent2->body()->addBoneBox(1, 0, "root2");
        BoneBox *leftBone = agent2->body()->addBoneBox(2, 1, "left2");
        BoneBox *rightBone = agent2->body()->addBoneBox(3, 1, "right2");
        BoneBox *topBone = agent2->body()->addBoneBox(4, 1, "top");
        rootBone->setTranslation(QVector3D(0, 0, 0));
        leftBone->setTranslation(QVector3D(10, 30, 0));
        rightBone->setTranslation(QVector3D(-10, 30, 0));
        topBone->setTranslation(QVector3D(0, 50, 0));
        // qDebug().noquote() << "Body QML :" << agent2->body()->skeletonQML();
    } {
        BoneBox *rootBone = agent3->body()->addBoneBox(1, 0, "root3");
        BoneBox *leftBone = agent3->body()->addBoneBox(2, 1, "left3");
        BoneBox *rightBone = agent3->body()->addBoneBox(3, 1, "right3");
        BoneBox *topBone = agent3->body()->addBoneBox(4, 1, "top3");
        rootBone->setTranslation(QVector3D(0, -100, 0));
        leftBone->setTranslation(QVector3D(10, 30, 0));
        rightBone->setTranslation(QVector3D(-10, 30, 0));
        topBone->setTranslation(QVector3D(0, 50, 0));
        // qDebug().noquote() << "Body QML :" << agent2->body()->skeletonQML();
    }

    // End Body

    auto generatorPoint = new GeneratorPoint(scene);
    generatorPoint->setCenterPoint(QVector3D(-1100, 0, 200));
    generatorPoint->setColumns(20);
    generatorPoint->setRows(20);
    generatorPoint->setDistance(90);
    generatorPoint->setNumTotalAgents(400);
    generatorPoint->addAgent(agent, 0);
    generatorPoint->addAgent(agent2, 1);
    generatorPoint->addAgent(agent3, 2);
    generatorPoint->updateLocators();

    generatorPoint->instanciateAgentInstances();

    auto generatorPoint2 = new GeneratorPoint(scene);
    generatorPoint2->setCenterPoint(QVector3D(2500, 0, -200));
    generatorPoint2->setColumns(20);
    generatorPoint2->setRows(20);
    generatorPoint2->setDistance(90);
    generatorPoint2->setNumTotalAgents(400);
    generatorPoint2->addAgent(agent, 0);
    generatorPoint2->addAgent(agent2, 1);
    generatorPoint2->addAgent(agent3, 2);
    generatorPoint2->updateLocators();

    generatorPoint2->instanciateAgentInstances();

    float i = 0.0;
    foreach(auto loc, generatorPoint->locators()) {
        if (loc->agentInstance()) {
            loc->agentInstance()
                    ->outputChannels()
                    .value(BrainiacGlobals::CO_TZ)
                    ->setValue(4 + i / 10);
            loc->agentInstance()
                    ->outputChannels()
                    .value(BrainiacGlobals::CO_RY)
                    ->setValue(2 + i / 10);
        }
        i += 1.0;
    }
    i = -5.5;
    foreach(auto loc, generatorPoint2->locators()) {
        if (loc->agentInstance()) {
            loc->agentInstance()
                    ->outputChannels()
                    .value(BrainiacGlobals::CO_TZ)
                    ->setValue(0.2 + i / 12);
            loc->agentInstance()
                    ->outputChannels()
                    .value(BrainiacGlobals::CO_RY)
                    ->setValue(1.2 + i / 12);
        }
        i += 0.2;
    }

    AgentInstance *myInstance = scene->agentInstances().constFirst();
    myInstance->outputChannels().value(BrainiacGlobals::CO_TZ)->setValue(4);
    myInstance->outputChannels().value(BrainiacGlobals::CO_RY)->setValue(1);
    AgentInstance *myInstance2 = scene->agentInstances().constLast();
    myInstance2->outputChannels().value(BrainiacGlobals::CO_TZ)->setValue(3);
    myInstance2->outputChannels().value(BrainiacGlobals::CO_RY)->setValue(-1);
    agent->setDefaultAgentInstance(myInstance);

    mainWindow.setMainEditor(agent->brain()->brainEditor());

    scene->simulation()->setEndFrame(1600);
    scene->simulation()->startSimulation();

    // Debug

    return app.exec();
}
