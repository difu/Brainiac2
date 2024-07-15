#include "bodyeditoritem.h"
#include "src/core/agent.h"
#include "src/core/brain/brain.h"
#include "src/core/brain/fuzzybase.h"

#include <QPainter>

BodyEditorItem::BodyEditorItem(Bone *parent)
    : EditorItem{parent} {
    qDebug() << "Created";
}

void BodyEditorItem::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    EditorItem::paint(painter, option, widget);

}
