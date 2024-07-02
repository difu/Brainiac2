#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>

#include <QString>

class AgentInstance;

/**
 * @class Channel
 * @brief The Channel class represents a channel in the agent instance.
 *
 * It manages the current and old values of the channel, and provides methods to set and get these values.
 * It also provides a method to reset the channel to its default values.
 */
class Channel : public QObject
{
    Q_OBJECT

public:
    /**
    * @struct ChannelDefaults
    * @brief This struct represents the default values for a channel.
    *
    * It includes the minimum, maximum, and default values of the channel.
    */
    struct ChannelDefaults
    {
        qreal min;   //!< the min value
        qreal max;   //!< the max value
        qreal value; //!< the default value of this channel
    };

    /**
     * @brief Channel constructor.
     *
     * This constructor creates a Channel object for the given agent instance and channel defaults.
     *
     * @param parent The agent instance to which this channel belongs.
     * @param defaults The default values for this channel.
     */
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
