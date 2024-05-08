#ifndef EDITORBASE_H
#define EDITORBASE_H

#include <QGraphicsScene>
#include <QObject>
#include <QWidget>

class EditorBase : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit EditorBase(QObject *parent);
};

#endif // EDITORBASE_H
