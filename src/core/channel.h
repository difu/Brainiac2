#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>

#include <QString>
#include "brainiacglobals.h"

class AgentInstance;

class Channel : public QObject
{
    Q_OBJECT
public:
    struct ChannelDefaults
    {
        qreal min;   //!< the min value
        qreal max;   //!< the max value
        qreal value; //!< the default value of this channel
    };
    explicit Channel(AgentInstance *parent, Channel::ChannelDefaults *defaults);

    ChannelDefaults *defaults() const;
    QString getInfo();

signals:

private:
    AgentInstance *m_agentInstance;
    ChannelDefaults *m_defaults;
    qreal m_value;        //!< Current value of this channel
    bool m_inherited;     //!< true, if this channel is inherited
    qreal m_oldValue;     //!< the old value of the channel
};

#endif // CHANNEL_H
