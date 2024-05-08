#ifndef BRAINEDITORITEM_H
#define BRAINEDITORITEM_H

#include "src/gui/editoritem.h"

class BrainEditorItem : public EditorItem
{
public:
    explicit BrainEditorItem(QObject *parent);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // BRAINEDITORITEM_H
