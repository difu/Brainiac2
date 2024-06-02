//
// Created by difu on 6/2/24.
//

#ifndef FUYYZAND_H
#define FUYYZAND_H

#include "../brainiacglobals.h"
#include "fuzzybase.h"

/**
 * @brief The FuzzyAnd class represents a fuzzy AND operator.
 *
 * The FuzzyAnd class is a subclass of FuzzyBase and implements the fuzzy AND operator.
 * It computes the AND operation on the inputs provided and returns the result.
 * The class provides various modes for computing the AND operation.
 *   - MIN: returns the minimum input value as the result
 *   - PRODUCT: returns the product of all input values as the result
 */
class FuzzyAnd : public FuzzyBase {
    Q_OBJECT
/**
 * @brief Constructs a new FuzzyAnd object.
 *
 * @param parent Pointer to the parent QObject. Default value is nullptr.
 * @param id The BrainiacId of the FuzzyOutput. Default value is 0.
 */
public:
    explicit FuzzyAnd(QObject *parent = nullptr, BrainiacGlobals::BrainiacId id=0);
    enum Mode { MIN,        //!< take the minimum input as result
                PRODUCT     //!< take the product of all inputs as result
              };
    void fromJson(QJsonObject obj) override;
    [[nodiscard]] QJsonObject toJson() const override;
    qreal result(const AgentInstance *agentInstance) override;

private:
    Mode m_mode;  //!< the mode of this FuzzyAnd
};

#endif //FUYYZAND_H
