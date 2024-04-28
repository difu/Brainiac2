//#include "../gui/mainwindow.h"

#include <QGuiApplication>
#include <QMetaObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QtQuick3D/qquick3d.h>
#include "agent.h"
#include "agentinstance.h"
#include "generator/generatormanual.h"
#include "generator/locator.h"
#include "scene.h"

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{file}:%{line} %{function} -> %{if-category}%{category}: %{endif}%{message}");

    QGuiApplication app(argc, argv);

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

    Scene *scene = new Scene;
    scene->setQQmlApplivationEngine(&engine);
    GeneratorManual *gen = new GeneratorManual(scene);
    Agent *agent = new Agent(scene);

    for (int i = 0; i < 15; i++) {
        Locator *loc = gen->addLocator(agent);
        loc->setLocation(QVector3D(50 + 100 * i, 0, 50 + 10 * i));
        loc->setRotation(QVector3D(0, 50 + i * 10, 0));
    }
    gen->apply();

    // Debug

    return app.exec();
}
