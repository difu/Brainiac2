#ifndef FUZZYBASE_H
#define FUZZYBASE_H

#include <QJsonObject>
#include <QObject>

#include "../brainiacglobals.h"

class AgentInstance;
class Brain;
class EditorItem;

class FuzzyBase : public QObject
{
    Q_OBJECT
public:
    enum LogicType {
        AND = BrainiacGlobals::AND,
        OR = BrainiacGlobals::OR,
        OUTPUT = BrainiacGlobals::OUTPUT,
        INPUT = BrainiacGlobals::INPUT,
        DEFUZZ = BrainiacGlobals::DEFUZZ,
        FUZZ = BrainiacGlobals::FUZZ,
        NOISE = BrainiacGlobals::NOISE,
        TIMER = BrainiacGlobals::TIMER
    };
    struct Parent
    {
        FuzzyBase *parent;
        bool inverted;
    };
    explicit FuzzyBase(QObject *parent = nullptr);

    qreal minValue() const;
    void setMinValue(qreal newMinValue);

    qreal maxValue() const;
    void setMaxValue(qreal newMaxValue);

    virtual qreal result(const AgentInstance *agentInstance) = 0;

    void addChild(FuzzyBase *child);
    QList<FuzzyBase *> children() const;

    void addParent(FuzzyBase *parent, bool isInverted = false);
    [[nodiscard]] QList<FuzzyBase::Parent> parents() const;

    QString name() const;
    void setName(const QString &newName);

    [[nodiscard]] BrainiacGlobals::BrainiacId id() const;

    QPointF editorPos() const;
    void setEditorPos(qreal x, qreal y);

    virtual void fromJson(QJsonObject obj) = 0;
    virtual QJsonObject toJson() const = 0;
    virtual ~FuzzyBase();

    EditorItem *editorItem() const;

    Brain *brain() const;

protected:
    QJsonObject getBaseJsonConfig();
    void setEditorItem(EditorItem *newEditorItem);
    void setId(BrainiacGlobals::BrainiacId newId);
    LogicType m_type;
    Brain *m_brain;
    EditorItem *m_editorItem;

private:
    qreal m_maxValue;
    qreal m_minValue;
    //qreal m_result;
    BrainiacGlobals::BrainiacId m_id;

private:
    QList<Parent> m_parents;
    QList<FuzzyBase *> m_children;

    QString m_name;

    Q_PROPERTY(qreal minValue READ minValue WRITE setMinValue NOTIFY minValueChanged FINAL)

    Q_PROPERTY(qreal maxValue READ maxValue WRITE setMaxValue NOTIFY maxValueChanged FINAL)

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)

signals:
    void minValueChanged();
    void maxValueChanged();
    void nameChanged();
};

#endif // FUZZYBASE_H
