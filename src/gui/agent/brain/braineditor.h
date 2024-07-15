#ifndef BRAINEDITOR_H
#define BRAINEDITOR_H

#include <QObject>
#include "../editorbase.h"

class BrainEditor : public EditorBase
{
    Q_OBJECT
public:
    explicit BrainEditor(QObject *parent);
    void deleteConnector(QObject *sourceObject, QObject *destObject);
};

#endif // BRAINEDITOR_H
