#ifndef NOISE_H
#define NOISE_H

#include <QObject>
#include "fuzzybase.h"

class Noise : public FuzzyBase
{
    Q_OBJECT
public:
    explicit Noise(QObject *parent = nullptr);

    void fromJson(QJsonObject obj);
    QJsonObject toJson() const;

    qreal rate() const;
    void setRate(qreal newRate);
    virtual qreal result(AgentInstance *agentInstance);

private:
    qreal m_rate; //!< the rate of this noise node
};

#endif // NOISE_H
