#include <QSignalSpy>
#include <QtTest>

#include "../src/core/scene.h"
#include "../src/core/simulation.h"

class SimulationTest : public QObject
{
    Q_OBJECT

public:
    SimulationTest();
    ~SimulationTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_basicSimulation();
};

SimulationTest::SimulationTest() {}

SimulationTest::~SimulationTest() {}

void SimulationTest::initTestCase() {}

void SimulationTest::cleanupTestCase() {}

void SimulationTest::test_basicSimulation()
{
    Scene the_scene;
    Simulation *sim = new Simulation(&the_scene);
    QSignalSpy spy_advanced(sim, SIGNAL(advanced(quint32)));

    sim->advance();
    QVERIFY2(sim->currentFrame() == 1, "First advanced frame should be 1.");
    QCOMPARE(spy_advanced.count(), 1); // make sure the signal was emitted exactly one time
    QList<QVariant> arguments = spy_advanced.takeFirst(); // take the first signal
    QVERIFY2(arguments.at(0).toInt() == 1,
             "Wrong frame number emitted."); // verify the first argument
    sim->reset();
    QVERIFY2(sim->currentFrame() == 0, "After reset current frame should be 0.");

    QSignalSpy spy_endReached(sim, SIGNAL(endFrameReached(void)));
    for (int i = 0; i < sim->endFrame(); i++) {
        sim->advance();
    }
    QCOMPARE(spy_endReached.count(), 1); // make sure the signal was emitted exactly one time
}

QTEST_MAIN(SimulationTest)

#include "tst_simulation.moc"
