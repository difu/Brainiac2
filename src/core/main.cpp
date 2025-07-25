//#include "../gui/mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QMetaObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQuick3D/qquick3d.h>
#include "agent.h"
#include "agentinstance.h"
#include "brain/brain.h"
#include "brain/noise.h"
#include "brain/fuzzyoutput.h"
#include "brain/fuzzyand.h"
#include "brainiacglobals.h"
#include "generator/locator.h"
#include "scene.h"
#include "simulation.h"
#include "body/body.h"
#include "brain/fuzzyor.h"
#include "generator/generatorpoint.h"
#include "src/gui/mainwindow.h"

// Function to create the hardcoded scene setup
void createHardcodedScene(Scene *scene);

int main(int argc, char *argv[]) {
    qSetMessagePattern("%{file}:%{line} %{type} %{function} -> %{if-category}%{category}: %{endif}%{message}");

    QApplication app(argc, argv);
    app.setApplicationName("Brainiac");
    app.setApplicationVersion("1.0");

    // Parse command line arguments
    QCommandLineParser parser;
    parser.setApplicationDescription("Brainiac - Agent-based simulation and visualization application");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption bsfOption(QStringList() << "bsf",
                                 "Load scene from BSF file",
                                 "filename");
    parser.addOption(bsfOption);

    parser.process(app);

    QString bsfFileName = parser.value(bsfOption);

    QSurfaceFormat::setDefaultFormat(QQuick3D::idealSurfaceFormat());

    QQmlApplicationEngine engine;
    QQmlContext context(&engine);
    const auto connection = QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    Q_UNUSED(connection);
    engine.loadFromModule("BrainiacViewer", "MainViewer");

    auto *scene = new Scene;
    MainWindow mainWindow(scene);
    mainWindow.setGeometry(100, 100, 800, 500);
    mainWindow.show();
    scene->setQQmlApplivationEngine(&engine);
    
    // Load scene from file or create hardcoded scene
    if (!bsfFileName.isEmpty()) {
        qDebug() << "Loading scene from BSF file:" << bsfFileName;
        scene->setFileName(bsfFileName);
        if (!scene->load()) {
            qFatal("Failed to load scene file: %s", qPrintable(bsfFileName));
        }
        qDebug() << "Successfully loaded scene with" << scene->agents().count() << "agents and" << scene->generators().
                count() << "generators";

        // Generate agent instances from loaded generators
        foreach(auto *generator, scene->generators()) {
            generator->updateLocators();
            generator->instanciateAgentInstances();
        }
    } else {
        qDebug() << "Creating hardcoded scene setup";
        createHardcodedScene(scene);
    }

    // Set default agent for brain editor (use first agent if available)
    Agent *defaultAgent = nullptr;
    if (!scene->agents().isEmpty()) {
        defaultAgent = scene->agents().first();
    }

    if (defaultAgent && defaultAgent->brain()) {
        mainWindow.setMainEditor(defaultAgent->brain()->brainEditor());
    }

    scene->simulation()->setEndFrame(1600);
    scene->simulation()->startSimulation();

    if (!scene->generators().isEmpty()) {
        float i = 0.0;
        foreach(auto *generator, scene->generators()) {
            foreach(auto *locator, generator->locators()) {
                if (locator->agentInstance()) {
                    locator->agentInstance()
                            ->outputChannels()
                            .value(BrainiacGlobals::CO_TZ)
                            ->setValue(4 + i / 10);
                    locator->agentInstance()
                            ->outputChannels()
                            .value(BrainiacGlobals::CO_RY)
                            ->setValue(2 + i / 10);
                }
            }
            i += 1.0;
        }
    }

    return app.exec();
}

void createHardcodedScene(Scene *scene) {
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
        rootBone->setSize(QVector3D(10, 10, 10));
        leftBone->setTranslation(QVector3D(30, -20, 0));
        leftBone->setSize(QVector3D(20, 10, 10));
        rightBone->setTranslation(QVector3D(-30, -20, 0));
        rightBone->setSize(QVector3D(20, 10, 30));
        // qDebug().noquote() << "Body QML :" << agent->body()->skeletonQML();
    }
    {
        BoneBox *rootBone = agent2->body()->addBoneBox(1, 0, "root2");
        BoneBox *leftBone = agent2->body()->addBoneBox(2, 1, "left2");
        BoneBox *rightBone = agent2->body()->addBoneBox(3, 1, "right2");
        BoneBox *topBone = agent2->body()->addBoneBox(4, 1, "top");
        rootBone->setTranslation(QVector3D(0, 0, 0));
        rootBone->setSize(QVector3D(40, 40, 40));
        leftBone->setTranslation(QVector3D(10, 30, 0));
        leftBone->setSize(QVector3D(10, 10, 10));
        rightBone->setTranslation(QVector3D(-10, 30, 0));
        rightBone->setSize(QVector3D(20, 10, 30));
        topBone->setTranslation(QVector3D(0, 50, 0));
        // qDebug().noquote() << "Body QML :" << agent2->body()->skeletonQML();
    } {
        BoneBox *rootBone = agent3->body()->addBoneBox(1, 0, "root3");
        BoneBox *leftBone = agent3->body()->addBoneBox(2, 1, "left3");
        BoneBox *rightBone = agent3->body()->addBoneBox(3, 1, "right3");
        BoneBox *topBone = agent3->body()->addBoneBox(4, 1, "top3");
        rootBone->setTranslation(QVector3D(0, -100, 0));
        rootBone->setSize(QVector3D(10, 10, 10));
        leftBone->setTranslation(QVector3D(10, 30, 0));
        leftBone->setSize(QVector3D(20, 10, 10));
        rightBone->setTranslation(QVector3D(-10, 30, 0));
        rightBone->setSize(QVector3D(20, 10, 30));
        topBone->setTranslation(QVector3D(0, 50, 0));
        topBone->setSize(QVector3D(10, 10, 10));
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
}
