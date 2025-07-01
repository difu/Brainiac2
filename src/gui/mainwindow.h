#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QShortcut>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include "src/core/scene.h"
#include "src/gui/editorbase.h"
#include "src/gui/agent/brain/braineditor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Scene *scene, QWidget *parent = nullptr);
    void setMainEditor(EditorBase *newEditor);
    ~MainWindow();

private slots:
    void selectBrainEditor();

private:
    //    Ui::MainWindow *ui;
    QGraphicsView *m_graphicsView;
    EditorBase *m_currentEditor;
    Scene *m_scene;
    QAction *m_brainEditorAction;

    void setupMenus();
};
#endif // MAINWINDOW_H
