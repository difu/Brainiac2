#ifndef FUZZYBASE_H
#define FUZZYBASE_H

#include <QObject>

class FuzzyBase : public QObject
{
    Q_OBJECT
public:
    explicit FuzzyBase(QObject *parent = nullptr);

    qreal minValue() const;
    void setMinValue(qreal newMinValue);

    qreal maxValue() const;
    void setMaxValue(qreal newMaxValue);

    void addChild(FuzzyBase *child);
    QList<FuzzyBase *> children() const;

    void addParent(FuzzyBase *parent, bool isInverted = false);

private:
    struct Parent
    {
        FuzzyBase *parent;
        bool inverted;
    };

    qreal m_maxValue;
    qreal m_minValue;
    qreal m_result;

    QList<Parent> m_parents;
    QList<FuzzyBase *> m_children;

    Q_PROPERTY(qreal minValue READ minValue WRITE setMinValue NOTIFY minValueChanged FINAL)

    Q_PROPERTY(qreal maxValue READ maxValue WRITE setMaxValue NOTIFY maxValueChanged FINAL)

signals:
    void minValueChanged();
    void maxValueChanged();
};

#endif // FUZZYBASE_H
