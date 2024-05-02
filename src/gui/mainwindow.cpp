#include "mainwindow.h"

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
}

MainWindow::~MainWindow()
{
    //    delete ui;
}
