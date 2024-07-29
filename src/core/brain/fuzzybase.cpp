#include "fuzzybase.h"
#include <QGraphicsItem>
#include "brain.h"
#include "src/gui/editoritem.h"
#include "src/gui/editoritemconnector.h"

FuzzyBase::FuzzyBase(LogicType type, QObject *parent)
    : QObject{parent}, m_type{type}
{

    m_id=0;
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

QList<FuzzyBase *> FuzzyBase::children() const
{
    return m_children;
}

void FuzzyBase::connectFuzzies(FuzzyBase *parent, FuzzyBase *child, const bool isInverted) {
    parent->addChild(child);
    child->addParent(parent, isInverted);
    const auto brain = parent->brain();
    const auto connector = new EditorItemConnector{
        parent->editorItem(),
        child->editorItem()
    };
    brain->brainEditor()->addItem(connector);
}

void FuzzyBase::addParent(FuzzyBase *parent, bool isInverted)
{
    Parent par{};
    par.parent = parent;
    par.inverted = isInverted;
    m_parents.append(par);
}

QList<FuzzyBase::Parent> FuzzyBase::parents() const
{
    return m_parents;
}

QString FuzzyBase::name() const
{
    return m_name;
}

void FuzzyBase::setName(const QString &newName)
{
    QString newNameNoBlanks = newName;
    newNameNoBlanks.replace(" ", "_");
    if (m_name == newNameNoBlanks)
        return;
    m_name = newNameNoBlanks;
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

FuzzyBase::~FuzzyBase() {}

EditorItem *FuzzyBase::editorItem() const
{
    return m_editorItem;
}

void FuzzyBase::setEditorItem(EditorItem *newEditorItem)
{
    m_editorItem = newEditorItem;
    m_brain->brainEditor()->addItem(m_editorItem);
}

BrainiacGlobals::BrainiacId FuzzyBase::id() const {
    return m_id;
}

void FuzzyBase::setId(BrainiacGlobals::BrainiacId newId) {
    m_id=newId;
}

FuzzyBase::LogicType FuzzyBase::type() const {
    return m_type;
}

Brain *FuzzyBase::brain() const
{
    return m_brain;
}
