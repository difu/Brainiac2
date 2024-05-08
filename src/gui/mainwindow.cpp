#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
//    , ui(new Ui::MainWindow)
{
    //    ui->setupUi(this);
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
    m_graphicsView->setScene(newEditor);

    foreach (QGraphicsItem *item, newEditor->items()) {
        qDebug() << item;
    }
    if (!newEditor->items().empty()) {
        QGraphicsItem *firstItem = newEditor->items().constFirst();
        m_graphicsView->centerOn(firstItem);
    }
    m_graphicsView->update();
}

MainWindow::~MainWindow()
{
    //    delete ui;
}
