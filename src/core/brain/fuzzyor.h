//
// Created by difu on 6/2/24.
//

#ifndef FUZZYOR_H
#define FUZZYOR_H

#include "../brainiacglobals.h"
#include "fuzzybase.h"

class FuzzyOr : public FuzzyBase {
    Q_OBJECT

public:
    explicit FuzzyOr(QObject *parent = nullptr, BrainiacGlobals::BrainiacId id=0);
    enum Mode { MAX,        //!< take the maximum input as result
                SUM         //!< take the sum of all inputs as result
              };
    void fromJson(QJsonObject obj) override;
    [[nodiscard]] QJsonObject toJson() const override;
    qreal result(const AgentInstance *agentInstance) override;

private:
    Mode m_mode;  //!< the mode of this FuzzyOr
};



#endif //FUZZYOR_H
