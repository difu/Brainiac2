#include "fuzzyoutput.h"

#include "agentinstancebrain.h"
#include "brain.h"
#include "brain/braineditoritem.h"
#include "src/core/agent.h"
#include "src/core/agentinstance.h"

FuzzyOutput::FuzzyOutput(QObject *parent, BrainiacGlobals::BrainiacId id)
    : FuzzyBase{FuzzyBase::OUTPUT, parent} {
    m_channelId = 0;
    setId(id);
    setMinValue(0.0);
    setMaxValue(1.0);
    this->setEditorItem(new BrainEditorItem(this));
}

void FuzzyOutput::fromJson(QJsonObject obj) {
}

QJsonObject FuzzyOutput::toJson() const {
    return {};
}

BrainiacGlobals::BrainiacId FuzzyOutput::channelId() const {
    return m_channelId;
}

void FuzzyOutput::setChannelId(BrainiacGlobals::BrainiacId newChannelId) {
    if (m_channelId == newChannelId) {
        return;
    }
    m_channelId = newChannelId;
    if (brain()->agent()->outputChannelDefaults().contains(m_channelId)) {
        setMinValue(brain()->agent()->outputChannelDefaults().value(m_channelId)->min);
        setMaxValue(brain()->agent()->outputChannelDefaults().value(m_channelId)->max);
    } else {
        qDebug() << "Channel with id " << newChannelId << " does not exist";
    }

    emit channelIdChanged();
}

qreal FuzzyOutput::result(const AgentInstance *agentInstance) {
    if (agentInstance->instanceBrain()->hasResult(this->id())) {
        return agentInstance->instanceBrain()->fuzzyResults().value(this->id());
    } else {
        if (m_channelId) {
            if (parents().count() == 0) {
                const qreal result = agentInstance->outputChannels().value(m_channelId)->value();
                agentInstance->instanceBrain()->setResult(this->id(), result);
                return result;
            } else {
                const qreal result = parents().at(0).parent->result(agentInstance);
                agentInstance->instanceBrain()->setResult(this->id(), result);
                agentInstance->outputChannels().value(m_channelId)->setValue(result);
                return result;
            }
        }
    }

    return 0;
}
