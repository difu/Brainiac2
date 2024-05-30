#ifndef FUZZYINPUT_H
#define FUZZYINPUT_H

#include <QObject>
#include "../brainiacglobals.h"
#include "fuzzybase.h"

/**
 * @class FuzzyInput
 * @brief The FuzzyInput class represents a fuzzy input in a fuzzy system.
 *        It inherits from the FuzzyBase class.
 *
 * The FuzzyInput class provides methods to handle fuzzy inputs, such
 * as converting to and from JSON, getting and setting the channel ID, and
 * calculating the result for a given agent instance.
 */
class FuzzyInput : public FuzzyBase
{
    Q_OBJECT
public:
    explicit FuzzyInput(QObject *parent = nullptr);

    void fromJson(QJsonObject obj);
    QJsonObject toJson() const;

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

#endif // FUZZYINPUT_H
