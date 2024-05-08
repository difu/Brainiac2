#include "mainwindow.h"
#include <QDebug>
#include "src/core/simulation.h"

MainWindow::MainWindow(Scene *scene, QWidget *parent)
    : QMainWindow(parent)
//    , ui(new Ui::MainWindow)
{
    //    ui->setupUi(this);
    m_scene = scene;
    m_currentEditor = nullptr;
    QHBoxLayout *layout = new QHBoxLayout;
    //layout->addWidget(toolBox);
    m_graphicsView = new QGraphicsView(this);
    layout->addWidget(m_graphicsView);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
}

void MainWindow::setMainEditor(EditorBase *newEditor)
{
    if (m_currentEditor != newEditor) {
        m_graphicsView->setScene(newEditor);

        foreach (QGraphicsItem *item, newEditor->items()) {
            qDebug() << item;
        }
        if (!newEditor->items().empty()) {
            QGraphicsItem *firstItem = newEditor->items().constFirst();
            m_graphicsView->centerOn(firstItem);
        }
        Simulation *sim = m_scene->simulation();
        if (m_currentEditor) {
            QObject::disconnect(sim, SIGNAL(advanced(quint32)), m_currentEditor, SLOT(update()));
        }
        m_currentEditor = newEditor;
        QObject::connect(sim, SIGNAL(advanced(quint32)), newEditor, SLOT(update()));
    }
    m_graphicsView->update();
}

MainWindow::~MainWindow()
{
    //    delete ui;
}
