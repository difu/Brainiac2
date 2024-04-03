#include "fuzzybase.h"

FuzzyBase::FuzzyBase(QObject *parent)
    : QObject{parent}
{}

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
