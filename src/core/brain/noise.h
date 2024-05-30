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

    /**
     * @brief Computes the result of the Noise function for a given Agent Instance.
     *
     * This function calculates the result of the Noise function for a given Agent Instance.
     *
     * @param agentInstance A pointer to the Agent Instance for which the result is to be computed.
     * @return The computed result of the Noise function.
     *         Returns 0.0 if the agentInstance is null or if the agentInstance does not have a valid Brain.
     *         Returns a previously computed result if it exists in the agentInstance's Brain.
     *         Otherwise, calculates and returns a new result based on the parameters of the Noise function.
     */
    virtual qreal result(const AgentInstance *agentInstance);

private:
    qreal m_rate; //!< the rate of this noise node
};

#endif // NOISE_H
