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
    QObject *viewer = engine.rootObjects().first();

    // qDebug() << viewer->metaObject()->className() << "#Children" << viewer->children().count();
    // foreach (QObject *obj, viewer->findChildren<QObject *>("agentInstanceSpawner")) {
    //     qDebug() << "Child: " << obj->metaObject()->className() << "#Children"
    //              << obj->children().count() << context.nameForObject(obj);
    //     QString message = "Hello from main.cpp";
    //     QVariant receive;
    //     QMetaObject::invokeMethod(obj,
    //                               "debugMsg",
    //                               Q_RETURN_ARG(QVariant, receive),
    //                               Q_ARG(QString, message));
    //     qDebug() << receive;
    // }

    // foreach (QObject *obj, viewer->findChildren<QObject *>("AgentInstaceGeometry")) {
    //     qDebug() << "Child: " << obj->metaObject()->className() << context.nameForObject(obj);
    //     AgentInstance *inst = new AgentInstance();
    //     QMetaObject::invokeMethod(obj, "setAgentInstance", Q_ARG(AgentInstance *, inst));
    // }

    Scene *scene = new Scene;
    GeneratorManual *gen = new GeneratorManual(scene);
    Agent *agent = new Agent();

    qDebug() << viewer->metaObject()->className() << "#Children" << viewer->children().count();
    foreach (QObject *obj, viewer->findChildren<QObject *>("agentInstanceSpawner")) {
        qDebug() << "Child: " << obj->metaObject()->className() << "  "
                 << context.nameForObject(obj);
        // for (int i = 0; i < 4; i++) {
        //     AgentInstance *inst = new AgentInstance();
        //     QPointer<AgentInstance> qp(inst);
        //     QVariant qv = QVariant::fromValue(qp);
        //     QMetaObject::invokeMethod(obj, "addAgentInstance", Q_ARG(QVariant, qv));
        //     qDebug() << "QVariant from main.cpp " << qv;
        // }

        Locator *loc1 = gen->addLocator(agent);
        qDebug() << "After addLocator";
        loc1->setLocation(QVector3D(50, -50, 50));
        //loc1->setRotation(QVector3D(1, -50, 50));
        qDebug() << "After setLocation";
        gen->apply();
        qDebug() << "After apply ";
        QPointer<AgentInstance> qp(loc1->agentInstance());
        QVariant qv = QVariant::fromValue(qp);
        qDebug() << "QVariant from main.cpp " << qv;
        QMetaObject::invokeMethod(obj, "addAgentInstance", Q_ARG(QVariant, qv));
        loc1->setLocation(QVector3D(0, 50, 0));
        loc1->setRotation(QVector3D(0, -50, 50));
        loc1->agentInstance()->reset();
    }

    // Debug

    return app.exec();
}
