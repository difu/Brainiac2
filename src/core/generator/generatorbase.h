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

    /**
     * @brief Updates the locators based on the current configuration.
     */
    virtual void updateLocators() = 0;

    Scene *scene();
    virtual ~GeneratorBase();

    [[nodiscard]] quint32 rows() const;

    void setRows(quint32 newRows);

    [[nodiscard]] quint32 columns() const;

    void setColumns(quint32 newColumns);

    [[nodiscard]] quint32 numTotalAgents() const;

    void setNumTotalAgents(quint32 newNumTotalAgents);

    [[nodiscard]] QHash<Agent *, qreal> agentRatios() const;

    [[nodiscard]] qreal distance() const;

    void setDistance(qreal newDistance);

    [[nodiscard]] qreal noise() const;

    void setNoise(qreal newNoise);

    [[nodiscard]] qreal angle() const;

    void setAngle(qreal newAngle);

    [[nodiscard]] qreal angleVariation() const;

    void setAngleVariation(qreal newAngleVariation);

    [[nodiscard]] qreal height() const;

    void setHeight(qreal newHeight);

    [[nodiscard]] qreal heightVariation() const;

    void setHeightVariation(qreal newHeightVariation);

    /**
     * @brief List of Locators.
     *
     * A QList of Locator pointers representing the locators associated with the GeneratorBase.
     *
     * @see Locator
     * @see GeneratorBase
     */
    qreal gap() const;

    void setGap(qreal newGap);

    QList<Agent *> agents() const;

protected:
    QList<Locator *> m_locators;

    /**
     * @brief Shuffles a list of numbers and returns the shuffled list.
     *
     * This method shuffles a list of numbers and returns the shuffled list. The size of the resulting
     * list is equal to the provided number of items.
     *
     * If the number of items exceeds the defined range, a fatal error is logged using qFatal().
     *
     * @param numberOfItems The number of items in the list.
     * @return The shuffled list of numbers.
     */
    static QList<quint32> shuffeldList(const quint32 numberOfItems);

    /**
     * @brief Removes the last n locators from the GeneratorBase.
     *
     * This method removes the last n locators from the list of locators associated with the GeneratorBase.
     * If n is greater than the size of the list, an error message is outputted using qDebug().
     *
     * IMPORTANT:
     * Also locators will be deleted, even if they are in LOCKED state!
     *
     * @param n The number of locators to remove.
     *
     * @see GeneratorBase
     * @see Locator
     */
    void removeLastNLocators(int n);

private:
    QHash<Agent *, qreal> m_agentRatios;
    QList<Agent *> m_agents;
    quint32 m_numTotalAgents;
    qreal m_gap;

    quint32 m_rows;
    quint32 m_columns;

    qreal m_distance;

    qreal m_noise;
    qreal m_angle;
    qreal m_angleVariation;

    qreal m_height;
    qreal m_heightVariation;

    void recalculateRatios();

signals:
};

#endif // GENERATORBASE_H
