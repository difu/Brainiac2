#include <QtTest>

// add necessary includes here
#include "src/core/agent.h"
#include "src/core/body/body.h"
// #include "../src/core/brain/brain.h"
// #include "../src/core/brainiacglobals.h"
#include <QGraphicsScene>

#include "src/core/scene.h"
#include "src/core/brain/brain.h"
#include "src/core/brain/fuzzybase.h"
#include "src/core/brain/fuzzyand.h"
#include "src/core/brain/fuzzyor.h"
#include "src/core/brain/noise.h"
#include "src/core/brain/fuzzyoutput.h"
#include "src/core/generator/generatormanual.h"

void testAgent1(Agent *agent)
{
    {
        BoneBox *rootBone = agent->body()->addBoneBox(1, 0, "root");
        BoneBox *leftBone = agent->body()->addBoneBox(2, 1, "left");
        BoneBox *rightBone = agent->body()->addBoneBox(3, 1, "right");
        rootBone->setTranslation(QVector3D(0, 0, 0));
        rootBone->setSize(QVector3D(10, 20, 30));
        rootBone->setEditorPos(100, 100);
        leftBone->setTranslation(QVector3D(30, -20, 0));
        rightBone->setTranslation(QVector3D(-30, -20, 0));
    } {
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
        newNoise3->setRate(0.5);
        newNoise3->setName("Color Noise");
        newNoise3->setEditorPos(100, 225);

        FuzzyBase::connectFuzzies(newNoise, newAnd);
        FuzzyBase::connectFuzzies(newNoise2, newAnd);
        FuzzyBase::connectFuzzies(newNoise, newOr);
        FuzzyBase::connectFuzzies(newNoise2, newOr);
        FuzzyBase::connectFuzzies(newNoise3, newOutput);
    }
}

class AgentTest : public QObject
{
    Q_OBJECT

public:
    AgentTest();
    ~AgentTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_loadSave();

    // void test_outputChannels();
    // void test_inputChannels();
};

AgentTest::AgentTest() {}

AgentTest::~AgentTest() {}

void AgentTest::initTestCase()
{
    qSetMessagePattern(
        "%{file}:%{line} %{function} -> %{if-category}%{category}: %{endif}%{message}");
}

void AgentTest::cleanupTestCase() {}

void AgentTest::test_loadSave()
{
    Scene myScene;

    auto *agent = new Agent(&myScene);
    ::testAgent1(agent);
    agent->setFileName("/tmp/difu.baf");
    agent->save();

    auto *loadAgent = new Agent(&myScene);
    loadAgent->setFileName("/tmp/difu.baf");
    loadAgent->load();

    loadAgent->setFileName("/tmp/difu2.baf");
    loadAgent->save();
}

// void AgentTest::test_outputChannels()
// {
//     Agent agent;
//     BrainiacGlobals::BrainiacId id1, id2;
//     quint32 numberOfChannels = agent.numOutputChannels();
//
//     Channel::ChannelDefaults channelDefaults;
//     channelDefaults.max = 123.0f;
//     channelDefaults.min = 456.0f;
//     channelDefaults.value = 7.0f;
//
//     id1 = agent.addOutputChannel("test1", channelDefaults);
//     QVERIFY2(id1 != 0, "Id should never be 0!");
//     quint32 newNumberOfChannels = agent.numOutputChannels();
//     QVERIFY2(numberOfChannels != newNumberOfChannels, "No new output channel was created!");
//
//     numberOfChannels = newNumberOfChannels;
//
//     // Test, if same channel can not be added again
//     id2 = agent.addOutputChannel("test1", channelDefaults);
//     QVERIFY2(id2 != 0, "Id should never be 0!");
//     QVERIFY2(id1 == id2, "The same channel must not be created again!");
//     newNumberOfChannels = agent.numOutputChannels();
//     QVERIFY2(numberOfChannels == newNumberOfChannels,
//              "Number of channels must not increase if same channel was tried to add!");
//
//     // Test, if already used id is reassigned to a new
//     id1 = agent.addOutputChannel("test1", channelDefaults, 1);
//     QVERIFY2(id1 != 0, "Id should never be 0!");
//     QVERIFY2(id1 != 1, "Id=1 is already in use!");
// }
//
// void AgentTest::test_inputChannels()
// {
//     Agent agent;
//     BrainiacGlobals::BrainiacId id1, id2;
//     quint32 numberOfChannels = agent.numInputChannels();
//
//     Channel::ChannelDefaults channelDefaults;
//     channelDefaults.max = 123.0f;
//     channelDefaults.min = 456.0f;
//     channelDefaults.value = 8.0f;
//
//     id1 = agent.addInputChannel("test1", channelDefaults);
//     quint32 newNumberOfChannels = agent.numInputChannels();
//     QVERIFY2(numberOfChannels != newNumberOfChannels, "No new output channel was created!");
//     QVERIFY2(id1 != 0, "Id should never be 0!");
//
//     numberOfChannels = newNumberOfChannels;
//
//     // Test, if same channel can not be added again
//     id2 = agent.addInputChannel("test1", channelDefaults);
//     QVERIFY2(id1 == id2, "The same channel must not be created again!");
//     QVERIFY2(id2 != 0, "Id should never be 0!");
//     newNumberOfChannels = agent.numInputChannels();
//     QVERIFY2(numberOfChannels == newNumberOfChannels,
//              "Number of channels must not increase if same channel was tried to add!");
//
//     // Test, if already used id is reassigned to a new
//     id1 = agent.addInputChannel("test1", channelDefaults, 1);
//     QVERIFY2(id1 != 0, "Id should never be 0!");
//     QVERIFY2(id1 != 1, "Id=1 is already in use!");
// }

QTEST_MAIN(AgentTest)

#include "tst_agent.moc"
