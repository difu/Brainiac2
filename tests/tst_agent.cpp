#include <QtTest>

// add necessary includes here
#include "../src/core/agent.h"
#include "../src/core/brain/brain.h"
#include "../src/core/brainiacglobals.h"

class AgentTest : public QObject
{
    Q_OBJECT

public:
    AgentTest();
    ~AgentTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
    void test_outputChannels();
    void test_inputChannels();
};

AgentTest::AgentTest() {}

AgentTest::~AgentTest() {}

void AgentTest::initTestCase() {}

void AgentTest::cleanupTestCase() {}

void AgentTest::test_case1() {}

void AgentTest::test_outputChannels()
{
    Agent agent;
    BrainiacGlobals::BrainiacId id1, id2;
    quint32 numberOfChannels = agent.numOutputChannels();

    Channel::ChannelDefaults channelDefaults;
    channelDefaults.max = 123.0f;
    channelDefaults.min = 456.0f;
    channelDefaults.value = 7.0f;

    id1 = agent.addOutputChannel("test1", channelDefaults);
    QVERIFY2(id1 != 0, "Id should never be 0!");
    quint32 newNumberOfChannels = agent.numOutputChannels();
    QVERIFY2(numberOfChannels != newNumberOfChannels, "No new output channel was created!");

    numberOfChannels = newNumberOfChannels;

    // Test, if same channel can not be added again
    id2 = agent.addOutputChannel("test1", channelDefaults);
    QVERIFY2(id2 != 0, "Id should never be 0!");
    QVERIFY2(id1 == id2, "The same channel must not be created again!");
    newNumberOfChannels = agent.numOutputChannels();
    QVERIFY2(numberOfChannels == newNumberOfChannels,
             "Number of channels must not increase if same channel was tried to add!");

    // Test, if already used id is reassigned to a new
    id1 = agent.addOutputChannel("test1", channelDefaults, 1);
    QVERIFY2(id1 != 0, "Id should never be 0!");
    QVERIFY2(id1 != 1, "Id=1 is already in use!");
}

void AgentTest::test_inputChannels()
{
    Agent agent;
    BrainiacGlobals::BrainiacId id1, id2;
    quint32 numberOfChannels = agent.numInputChannels();

    Channel::ChannelDefaults channelDefaults;
    channelDefaults.max = 123.0f;
    channelDefaults.min = 456.0f;
    channelDefaults.value = 8.0f;

    id1 = agent.addInputChannel("test1", channelDefaults);
    quint32 newNumberOfChannels = agent.numInputChannels();
    QVERIFY2(numberOfChannels != newNumberOfChannels, "No new output channel was created!");
    QVERIFY2(id1 != 0, "Id should never be 0!");

    numberOfChannels = newNumberOfChannels;

    // Test, if same channel can not be added again
    id2 = agent.addInputChannel("test1", channelDefaults);
    QVERIFY2(id1 == id2, "The same channel must not be created again!");
    QVERIFY2(id2 != 0, "Id should never be 0!");
    newNumberOfChannels = agent.numInputChannels();
    QVERIFY2(numberOfChannels == newNumberOfChannels,
             "Number of channels must not increase if same channel was tried to add!");

    // Test, if already used id is reassigned to a new
    id1 = agent.addInputChannel("test1", channelDefaults, 1);
    QVERIFY2(id1 != 0, "Id should never be 0!");
    QVERIFY2(id1 != 1, "Id=1 is already in use!");
}

QTEST_APPLESS_MAIN(AgentTest)

#include "tst_agent.moc"
