#include "noise.h"

#include "agentinstancebrain.h"
#include "../agent.h"
#include "../agentinstance.h"
#include "../brainiacglobals.h"
#include "../generator/locator.h"
#include "../scene.h"
#include "../simulation.h"
#include "brain.h"
#include "src/gui/agent/brain/braineditoritem.h"

Noise::Noise(QObject *parent, BrainiacGlobals::BrainiacId id)
    : FuzzyBase{parent}
{
    m_type = LogicType::NOISE;
    m_rate = 0;
    setId(id);
    setMinValue(0.0);
    setMaxValue(1.0);
    this->setEditorItem(new BrainEditorItem(this));
}

void Noise::fromJson(QJsonObject obj) {}

QJsonObject Noise::toJson() const
{
    return QJsonObject();
}

qreal Noise::rate() const
{
    return m_rate;
}

void Noise::setRate(qreal newRate)
{
    m_rate = newRate;
}

qreal Noise::result(const AgentInstance *agentInstance)
{
    if (!agentInstance) {
        qDebug() << "No valid agentInstance";
        return 0.0;
    }
    if(agentInstance->instanceBrain()->hasResult(this->id())) {
        return agentInstance->instanceBrain()->fuzzyResults().value(this->id());
    }
    qreal result=0.0;
    const quint32 seed = agentInstance->locator()->seed();
    if (m_rate != 0.0) {
        const quint32 currentFrame = m_brain->agent()->scene()->simulation()->currentFrame();
        const qreal fps = m_brain->agent()->scene()->simulation()->frameRate();
        quint32 i = 1;
        while (fps * m_rate * i < currentFrame) {
            i++;
        }
        const quint32 lastRandFrame = (i - 1) * (fps * m_rate);
        const quint32 nextRandFrame = (i) * (fps * m_rate);

        const qreal lastRand = BrainiacGlobals::getRand(seed + lastRandFrame);
        const qreal nextRand = BrainiacGlobals::getRand(seed + nextRandFrame);
        const qreal diff = nextRand - lastRand;
        const qreal value = lastRand + (diff * (currentFrame - lastRandFrame) / (fps * m_rate));
        result = value;
    } else {
        result = BrainiacGlobals::getRand(seed);
    }
    agentInstance->instanceBrain()->setResult(this->id(), result);
    return result;
}
