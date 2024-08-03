#include <QSignalSpy>
#include <QtTest>

#include "../src/core/scene.h"
#include "src/core/agent.h"
#include "src/core/generator/generatorpoint.h"

void createTestAgent1(Agent *agent) {
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
    saveScene.setFileName(QString(rootTestDir.path()).append("/scene.bsf"));
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
}

QTEST_MAIN(SceneTest)

#include "tst_scene.moc"
