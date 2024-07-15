#ifndef BDOYEDITORITEM_H
#define BODYEDITORITEM_H

#include "src/gui/editoritem.h"
#include "src/core/body/bone.h"

class BodyEditorItem : public EditorItem
{
public:
    explicit BodyEditorItem(Bone *parent);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // BODYEDITORITEM_H
