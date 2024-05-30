#ifndef BRAIN_H
#define BRAIN_H

#include <QHash>
#include <QObject>
#include <QString>

#include "fuzzyoutput.h"
#include "../brainiacglobals.h"
#include "brain/braineditor.h"
#include "src/core/brain/noise.h"

class Agent;
class FuzzyBase;

/**
 * @class Brain
 * @brief This class represents the brain of an agent in the system.
 *
 * The Brain class is a QObject that encapsulates the functionality of an agent's brain in the system.
 * It provides methods for managing fuzzy logic components, creating new IDs, and accessing the agent and brain editor associated with the brain.
 * It also provides access to the agent's fuzzy logic components through a QHash.
 *
 * @note This class is part of the core functionality of the system and is used extensively throughout the codebase.
 */
class Brain : public QObject {
    class Brain : public QObject {
        Q_OBJECT

    public:
        /**
         * @brief Constructs a Brain object with the given parent.
         *
         * The parent of a Brain is the Agent
         *
         * @param parent The parent object of the Brain.
         * @return A new instance of Brain.
         */
        Brain(QObject *parent = nullptr);

        QHash<BrainiacGlobals::BrainiacId, FuzzyBase *> fuzzies() const;

        Agent *agent() const;

    Noise *addNoiseNode(BrainiacGlobals::BrainiacId id=0);
    FuzzyOutput *addOutputNode(BrainiacGlobals::BrainiacId id=0);
    BrainiacGlobals::BrainiacId newId();

    BrainEditor *brainEditor() const;

private:
    Agent *m_agent;
    QHash<BrainiacGlobals::BrainiacId, FuzzyBase *> m_fuzzies;
    BrainEditor *m_brainEditor;
};

#endif // BRAIN_H
