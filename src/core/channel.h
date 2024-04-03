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
        qreal min;
        qreal max;
        qreal value;
    };
    explicit Channel(AgentInstance *parent, Channel::ChannelDefaults *defaults);

    ChannelDefaults *defaults() const;
    QString getInfo();

signals:

private:
    AgentInstance *m_agentInstance;
    ChannelDefaults *m_defaults;
    qreal m_defaultValue; //!< the default value of this channel
    qreal m_value;        //!< Current value of this channel
    bool m_inherited;     //!< true, if this channel is inherited
    qreal m_oldValue;     //!< the old value of the channel
    qreal m_max;          //!< the max value
    qreal m_min;          //!< the min value
};

#endif // CHANNEL_H
