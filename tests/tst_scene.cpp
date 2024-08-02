#include <QSignalSpy>
#include <QtTest>

#include "../src/core/scene.h"

class SceneTest : public QObject {
    Q_OBJECT

public:
    SceneTest();

    ~SceneTest();

private slots:
    void initTestCase();

    void cleanupTestCase();

    void test_basicScene();
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
    Scene the_scene;
    QVERIFY2(false, "Fail fast!");
}

QTEST_MAIN(SceneTest)

#include "tst_scene.moc"
