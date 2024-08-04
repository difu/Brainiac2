#include <QSignalSpy>
#include <QtTest>

#include "../src/core/scene.h"
#include "src/core/agent.h"
#include "src/core/body/body.h"
#include "src/core/body/bonebox.h"
#include "src/core/generator/generatorpoint.h"

void createTestAgent1(Agent *agent) {
    {
        BoneBox *rootBone = agent->body()->addBoneBox(1, 0, "root");
        BoneBox *leftBone = agent->body()->addBoneBox(2, 1, "left");
        BoneBox *rightBone = agent->body()->addBoneBox(3, 1, "right");
        rootBone->setTranslation(QVector3D(0, 0, 0));
        rootBone->setSize(QVector3D(10, 20, 30));
        rootBone->setEditorPos(100, 100);
        leftBone->setTranslation(QVector3D(30, -20, 0));
        rightBone->setTranslation(QVector3D(-30, -20, 0));
    }
}

void createTestScene1(Scene *scene) {
    auto *pointGen1 = new GeneratorPoint(scene);
    auto *agent1 = new Agent(scene);
    auto *agent2 = new Agent(scene);
    createTestAgent1(agent1);
    createTestAgent1(agent2);

    pointGen1->addAgent(agent1, 0);
    pointGen1->addAgent(agent2, 1);
    pointGen1->setCenterPoint(QVector3D(1, 2, 3));
    pointGen1->setColumns(4);
    pointGen1->setRows(3);
    pointGen1->setNumTotalAgents(10);
    pointGen1->setAngle(2);
    pointGen1->setAngleVariation(3);
    pointGen1->setHeightVariation(4);
    pointGen1->setHeight(-5);
    pointGen1->setNoise(0.69);
    pointGen1->updateLocators();
}

class SceneTest : public QObject {
    Q_OBJECT

public:
    SceneTest();

    ~SceneTest();

private slots:
    void initTestCase();

    void cleanupTestCase();

    void test_basicScene();

    void test_loadSaveScene();
};

SceneTest::SceneTest() {
}

SceneTest::~SceneTest() {
}

void SceneTest::initTestCase() {
}

void SceneTest::cleanupTestCase() {
}

void SceneTest::test_basicScene() {
    Scene theScene;
    ::createTestScene1(&theScene);
    QVERIFY2(theScene.agents().count()==2, "Too few or too many agents found!");
}

void SceneTest::test_loadSaveScene() {
    QTemporaryDir rootTestDir;
    QDir agentFileDir;
    QString agentFileDirName = QString(rootTestDir.path().append("/agent_files"));
    if (!agentFileDir.mkdir(agentFileDirName)) {
        qCritical("Could not create file structure!");
    }
    agentFileDir.setPath(agentFileDirName);
    qInfo() << "Testfiles will be written to: " << rootTestDir.path();
    Scene saveScene;
    ::createTestScene1(&saveScene);
    auto sceneFileName = QString(rootTestDir.path()).append("/scene.bsf");
    saveScene.setFileName(sceneFileName);
    const int numOfAgents = saveScene.agents().count();
    QVERIFY2(numOfAgents==2, "Unexpected number of agents!");
    int agentIndex = 0;
    QList<QString> agentNames;
    foreach(auto *agent, saveScene.agents()) {
        QString agentName = QString("Agent").append(QString::number(agentIndex));
        agentNames.append(agentName);
        bool setOk = agent->setName(agentName);
        QString fileName = QString(agentFileDir.path()).append("/").append(agentName).append(".baf");
        agent->setFileName(fileName);
        QVERIFY2(agent->save(), "Could not save agent!");
        agentIndex++;
    }
    QVERIFY2(saveScene.save(), "Could not save scene!");
    Scene loadScene;
    loadScene.setFileName(sceneFileName);
    loadScene.load();
    QStringList errors;
    QVERIFY2(saveScene.agents().count() == loadScene.agents().count(),
             "Different number of agents in loaded scene!");
    bool equal = Scene::compare(&saveScene, &loadScene, errors);
    if (!equal) {
        qInfo() << errors;
    }
    QVERIFY2(equal, "Scenes are not the same!");
}

QTEST_MAIN(SceneTest)

#include "tst_scene.moc"
