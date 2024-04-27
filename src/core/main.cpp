//#include "../gui/mainwindow.h"

#include <QGuiApplication>
#include <QMetaObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QtQuick3D/qquick3d.h>
#include "agent.h"
#include "agentinstance.h"

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{file}:%{line} %{function} -> %{if-category}%{category}: %{endif}%{message}");
    //    QApplication a(argc, argv);
    //    MainWindow w;
    //    w.show();
    // Agent agent;

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

    qDebug() << viewer->metaObject()->className() << "#Children" << viewer->children().count();
    foreach (QObject *obj, viewer->findChildren<QObject *>("agentInstanceSpawner")) {
        qDebug() << "Child: " << obj->metaObject()->className() << "  "
                 << context.nameForObject(obj);
        for (int i = 0; i < 4; i++) {
            AgentInstance *inst = new AgentInstance();
            QPointer<AgentInstance> qp(inst);
            QVariant qv = QVariant::fromValue(qp);
            QMetaObject::invokeMethod(obj, "addAgentInstance", Q_ARG(QVariant, qv));
            qDebug() << "QVariant from main.cpp " << qv;
        }
    }

    // Debug

    return app.exec();
}
