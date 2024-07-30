#ifndef GENERATORBASE_H
#define GENERATORBASE_H

#include <QHash>
#include <QList>
#include <QObject>

class Agent;
class Locator;
class Scene;

class GeneratorBase : public QObject
{
    Q_OBJECT
public:
    explicit GeneratorBase(QObject *parent);

    void addAgent(Agent *newAgent, qsizetype position);

    void removeAgent(Agent *agent);

    virtual void apply() = 0;

    [[nodiscard]] QList<Locator *> locators() const;

    Scene *scene();
    virtual ~GeneratorBase();

protected:
    QList<Locator *> m_locators;
    QHash<Agent *, qreal> m_agentRatios;
    QList<Agent *> m_agents;
    quint32 m_numTotalAgents;

private:
    void recalculateRatios();

signals:
};

#endif // GENERATORBASE_H
