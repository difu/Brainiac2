#ifndef FUZZYOUTPUT_H
#define FUZZYOUTPUT_H

#include <QObject>
#include "../brainiacglobals.h"
#include "fuzzybase.h"

class FuzzyOutput : public FuzzyBase
{
    Q_OBJECT
public:
    explicit FuzzyOutput(QObject *parent = nullptr);

    void fromJson(QJsonObject obj);
    QJsonObject toJson() const;
    virtual qreal result(AgentInstance *agentInstance);

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
