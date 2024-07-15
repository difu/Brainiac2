#include "braineditor.h"
#include "qgraphicsitem.h"
#include "src/gui/editoritem.h"
#include "src/gui/editoritemconnector.h"

BrainEditor::BrainEditor(QObject *parent)
    : EditorBase{parent}
{
    //setSceneRect(QRectF(0, 0, 5000, 5000));
}
void BrainEditor::deleteConnector(QObject *sourceObject, QObject *destObject)
{
    foreach (QGraphicsItem *item, this->items()) {
        if (item->type() == EditorItemConnector::Type) {
            auto *conn = (EditorItemConnector *) item;
            if (conn->startItem()->object() == sourceObject
                && conn->endItem()->object() == destObject) {
                conn->startItem()->removeConnector(conn);
                conn->endItem()->removeConnector(conn);
                removeItem(conn);
                delete (conn);
                return;
            }
        }
    }
}
