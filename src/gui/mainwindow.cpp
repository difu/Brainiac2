#include "mainwindow.h"
#include <QDebug>

#include "src/core/agent.h"
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

    auto *quitShortcut = new QShortcut(QKeySequence::Quit, this);
    connect(quitShortcut, &QShortcut::activated, QApplication::instance(), &QApplication::quit);

    // Setup menus
    setupMenus();
}

void MainWindow::setMainEditor(EditorBase *newEditor)
{
    if (m_currentEditor != newEditor) {
        m_graphicsView->setScene(newEditor);

        foreach (QGraphicsItem *item, newEditor->items()) {
            // qDebug() << item;
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

void MainWindow::setupMenus() {
    QMenuBar *menuBar = this->menuBar();
    QMenu *editorMenu = menuBar->addMenu("&Editor");

    m_brainEditorAction = editorMenu->addAction("&Brain Editor");
    m_brainEditorAction->setCheckable(true);
    connect(m_brainEditorAction, &QAction::triggered, this, &MainWindow::selectBrainEditor);
}

void MainWindow::selectBrainEditor() {
    // Ensure only one editor is selected
    m_brainEditorAction->setChecked(true);

    // Get the first agent's brain editor (if available)
    if (!m_scene->agents().isEmpty()) {
        Agent *firstAgent = m_scene->agents().first();
        if (firstAgent && firstAgent->brain()) {
            setMainEditor(firstAgent->brain()->brainEditor());
        }
    }
}

MainWindow::~MainWindow()
{
    //    delete ui;
}
