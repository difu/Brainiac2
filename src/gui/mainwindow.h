#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "src/gui/editorbase.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void setMainEditor(EditorBase *newEditor);
    ~MainWindow();

private:
    //    Ui::MainWindow *ui;
    QGraphicsView *m_graphicsView;
};
#endif // MAINWINDOW_H
