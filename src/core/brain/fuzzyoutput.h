#ifndef FUZZYOUTPUT_H
#define FUZZYOUTPUT_H

#include <QObject>
#include "../brainiacglobals.h"
#include "fuzzybase.h"

class FuzzyOutput : public FuzzyBase
{
    Q_OBJECT
/**
 * @brief Constructs a new FuzzyOutput object.
 *
 * @param parent Pointer to the parent QObject. Default value is nullptr.
 * @param id The BrainiacId of the FuzzyOutput. Default value is 0.
 */
public:
    explicit FuzzyOutput(QObject *parent = nullptr, BrainiacGlobals::BrainiacId id=0);

    void fromJson(QJsonObject obj);
    QJsonObject toJson() const;

    /**
     * @brief Calculates the result of the FuzzyOutput for the given AgentInstance.
     *
     * The method checks if the AgentInstance's instanceBrain has a result for the FuzzyOutput.
     * If a result exists, it is returned.
     * If no result exists and the FuzzyOutput has a channel id specified,
     * the method checks if there are no parents.
     * If there are no parents, it retrieves the value from the output channel
     * with the specified id in the AgentInstance, sets the result in the instanceBrain,
     * and returns the value.
     *
     * @param agentInstance Pointer to the AgentInstance for which to calculate the result.
     * @return The calculated result of the FuzzyOutput.
     */
    qreal result(const AgentInstance *agentInstance) override;

    BrainiacGlobals::BrainiacId channelId() const;
    void setChannelId(BrainiacGlobals::BrainiacId newChannelId);

signals:
    void channelIdChanged();

private:
    BrainiacGlobals::BrainiacId m_channelId;
    Q_PROPERTY(BrainiacGlobals::BrainiacId channelId READ channelId WRITE setChannelId NOTIFY
                   channelIdChanged FINAL)
};

#endif // FUZZYOUTPUT_H
