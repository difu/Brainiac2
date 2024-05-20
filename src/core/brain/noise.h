#ifndef NOISE_H
#define NOISE_H

#include <QObject>
#include "fuzzybase.h"

/*!
 * \brief The Noise class
 * 
 * The parent is the Brain.
 */
class Noise : public FuzzyBase
{
    Q_OBJECT
public:
    explicit Noise(QObject *parent = nullptr, BrainiacGlobals::BrainiacId id=0);

    void fromJson(QJsonObject obj);
    QJsonObject toJson() const;

    /*!
     * @brief returns the rate of this noise node
     * @return the rate in Hz
     */
    qreal rate() const;
    void setRate(qreal newRate);
    virtual qreal result(const AgentInstance *agentInstance);

private:
    qreal m_rate; //!< the rate of this noise node
};

#endif // NOISE_H
