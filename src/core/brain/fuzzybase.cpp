#include "fuzzybase.h"
#include <QGraphicsItem>
#include "brain.h"
#include "src/gui/editoritem.h"

FuzzyBase::FuzzyBase(QObject *parent)
    : QObject{parent}
{
    m_brain = qobject_cast<Brain *>(parent);
    if (!m_brain) {
        qFatal() << "Parent of Fuzzy must be Brain!";
    }
}

qreal FuzzyBase::minValue() const
{
    return m_minValue;
}

void FuzzyBase::setMinValue(qreal newMinValue)
{
    if (qFuzzyCompare(m_minValue, newMinValue))
        return;
    m_minValue = newMinValue;
    emit minValueChanged();
}

qreal FuzzyBase::maxValue() const
{
    return m_maxValue;
}

void FuzzyBase::setMaxValue(qreal newMaxValue)
{
    if (qFuzzyCompare(m_maxValue, newMaxValue))
        return;
    m_maxValue = newMaxValue;
    emit maxValueChanged();
}

void FuzzyBase::addChild(FuzzyBase *child)
{
    m_children.append(child);
}

void FuzzyBase::addParent(FuzzyBase *parent, bool isInverted)
{
    Parent par;
    par.parent = parent;
    par.inverted = isInverted;
    m_parents.append(par);
}

QString FuzzyBase::name() const
{
    return m_name;
}

void FuzzyBase::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

QPointF FuzzyBase::editorPos() const
{
    return m_editorItem->pos();
}

void FuzzyBase::setEditorPos(qreal x, qreal y)
{
    m_editorItem->setPos(x, y);
}

FuzzyBase::~FuzzyBase()
{
    delete m_editorItem;
}

EditorItem *FuzzyBase::editorItem() const
{
    return m_editorItem;
}

void FuzzyBase::setEditorItem(EditorItem *newEditorItem)
{
    m_editorItem = newEditorItem;
    m_brain->brainEditor()->addItem(m_editorItem);
}

Brain *FuzzyBase::brain() const
{
    return m_brain;
}
