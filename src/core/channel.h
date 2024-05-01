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

    /** \brief set channels old value to actual value
    **/
    void advance();

    ChannelDefaults *defaults() const;

    /** \brief @returns the current value of this channel
    **/
    qreal value() const;

    /** \brief sets channel´s value

        set the channels value.
        the value is cropped to the channels limits

    **/
    void setValue(qreal value, bool isSpeed=false);

    /** \brief @returns the old value of this channel
    **/
    qreal oldValue() const;

    /** \brief resets the channel
     * the default value is applied both for value and old value
    **/
    void reset();

    /**
     * @brief getInfo
     * returns a QString containing the name and type of this channel
     *
     * @return QString info message
     */
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
