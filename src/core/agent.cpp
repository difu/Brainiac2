#include "agent.h"
#include "agentinstance.h"
#include "brain/brain.h"

#include <QtDebug>
#include <QtGlobal>

#include "scene.h"

Agent::Agent(Scene *parent)
    : QObject{parent}
{
    m_scene = parent;
    m_defaultAgentInstance = nullptr;

    m_brain = new Brain(this);
    Channel::ChannelDefaults trans_defaults;

    trans_defaults.max = 100;
    trans_defaults.min = -100;
    trans_defaults.value = 0.0f;

    Channel::ChannelDefaults rot_defaults;
    rot_defaults.max = 180.0f;
    rot_defaults.min = -180.0f;
    rot_defaults.value = 0.0f;

    addOutputChannel(QString("tx"), trans_defaults.min,trans_defaults.max, trans_defaults.value, BrainiacGlobals::TX);
    addOutputChannel(QString("ty"), trans_defaults.min,trans_defaults.max, trans_defaults.value, BrainiacGlobals::TY);
    addOutputChannel(QString("tz"), trans_defaults.min,trans_defaults.max, trans_defaults.value, BrainiacGlobals::TZ);

    addOutputChannel(QString("rx"), rot_defaults.min,rot_defaults.max, rot_defaults.value, BrainiacGlobals::RX);
    addOutputChannel(QString("ry"), rot_defaults.min,rot_defaults.max, rot_defaults.value, BrainiacGlobals::RY);
    addOutputChannel(QString("rz"), rot_defaults.min,rot_defaults.max, rot_defaults.value, BrainiacGlobals::RZ);

    addInputChannel(QString("tx"), trans_defaults.min,trans_defaults.max, trans_defaults.value, BrainiacGlobals::TX);
    addInputChannel(QString("ty"), trans_defaults.min,trans_defaults.max, trans_defaults.value, BrainiacGlobals::TY);
    addInputChannel(QString("tz"), trans_defaults.min,trans_defaults.max, trans_defaults.value, BrainiacGlobals::TY);

    addInputChannel(QString("rx"), trans_defaults.min,trans_defaults.max, trans_defaults.value, BrainiacGlobals::RX);
    addInputChannel(QString("ry"), trans_defaults.min,trans_defaults.max, trans_defaults.value, BrainiacGlobals::RY);
    addInputChannel(QString("rz"), trans_defaults.min,trans_defaults.max, trans_defaults.value, BrainiacGlobals::RZ);
}

AgentInstance *Agent::addAgentInstance(Locator *locator)
{
    AgentInstance *newAgentInstance = new AgentInstance(locator, this);
    m_agentInstances.append(newAgentInstance);
    foreach(BrainiacGlobals::BrainiacId inputId, m_inputChannels) {
        Channel::ChannelDefaults *channeldefaults = m_inputChannelDefaults.value(inputId);
        newAgentInstance->addInputChannel(inputId, channeldefaults);
    }
    foreach(BrainiacGlobals::BrainiacId outputId, m_outputChannels) {
        Channel::ChannelDefaults *channeldefaults = m_outputChannelDefaults.value(outputId);
        newAgentInstance->addOutputChannel(outputId, channeldefaults);
    }
    return newAgentInstance;
}

QString Agent::name() const
{
    return m_name;
}

void Agent::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

AgentInstance *Agent::defaultAgentInstance() const
{
    return m_defaultAgentInstance;
}

void Agent::setDefaultAgentInstance(AgentInstance *newDefaultAgentInstance)
{
    m_defaultAgentInstance = newDefaultAgentInstance;
}

Brain *Agent::brain() const
{
    return m_brain;
}

QList<AgentInstance *> Agent::agentInstances() const
{
    return m_agentInstances;
}

QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults *> Agent::inputChannelDefaults() const
{
    return m_inputChannelDefaults;
}

QString Agent::fileName() const
{
    return m_fileName;
}

bool Agent::load()
{
    return false;
}

bool Agent::save()
{
    return false;
}

QJsonObject Agent::toJson() const
{
    QJsonObject obj;
    obj["name"] = m_name;
    return obj;
}

Scene *Agent::scene() const
{
    return m_scene;
}

void Agent::setFileName(const QString &newFileName)
{
    m_fileName = newFileName;
}

BrainiacGlobals::BrainiacId Agent::addInputChannel(const QString channelName,
                                                   qreal channelMinVal, qreal channelMaxVal, qreal channelDefaultVal,
                                                   BrainiacGlobals::BrainiacId id)
{
    // if channel already exists, return its ID
    if (m_inputChannels.contains(channelName)) {
        qDebug() << __PRETTY_FUNCTION__ << "Channel " << channelName << "already exists.";
        return m_inputChannels.value(channelName);
    }

    BrainiacGlobals::BrainiacId maxId = 0;
    BrainiacGlobals::BrainiacId returnId = id;

    bool idFound = false;

    QHashIterator<QString, BrainiacGlobals::BrainiacId> i(m_inputChannels);
    while (i.hasNext()) {
        i.next();
        if (maxId < i.value()) {
            maxId = i.value();
        }
        if (id == i.value()) {
            idFound = true;
        }
    }
    maxId++;

    if (id == 0) { // requested a new ID
        returnId = maxId;
    }
    if (idFound) {
        qWarning() << __PRETTY_FUNCTION__ << " ID " << id << " was already used! New id is "
                   << maxId;
        returnId = maxId;
    }
    if (returnId == 0) {
        qCritical() << __PRETTY_FUNCTION__
                    << "This should never happen! ChannelName: " << channelName;
    }

    Channel::ChannelDefaults *cDefault = new Channel::ChannelDefaults;
    cDefault->min=channelMinVal;
    cDefault->max=channelMaxVal;
    cDefault->value=channelDefaultVal;

    m_inputChannels.insert(channelName, returnId);
    m_inputChannelDefaults.insert(returnId, cDefault);
    foreach (AgentInstance *agentInstance, m_agentInstances) {
        agentInstance->addInputChannel(returnId, cDefault);
    }
    if (m_defaultAgentInstance) {
        Q_ASSERT(m_inputChannels.count() == m_defaultAgentInstance->inputChannels().count());
    }
    Q_ASSERT(m_inputChannels.count() == m_inputChannelDefaults.count());
    return returnId;
}

BrainiacGlobals::BrainiacId Agent::addOutputChannel(const QString channelName,
                                                    qreal channelMinVal, qreal channelMaxVal, qreal channelDefaultVal,
                                                    BrainiacGlobals::BrainiacId id)
{
    // if channel already exists, return its ID
    if (m_outputChannels.contains(channelName)) {
        return m_outputChannels.value(channelName);
        qDebug() << __PRETTY_FUNCTION__ << "Channel " << channelName << "already exists.";
    }

    BrainiacGlobals::BrainiacId maxId = 0;
    BrainiacGlobals::BrainiacId returnId = id;

    bool idFound = false;

    QHashIterator<QString, BrainiacGlobals::BrainiacId> i(m_outputChannels);
    while (i.hasNext()) {
        i.next();
        if (maxId < i.value()) {
            maxId = i.value();
        }
        if (id == i.value()) {
            idFound = true;
        }
    }
    maxId++;

    if (id == 0) { // requested a new ID
        returnId = maxId;
    }
    if (idFound) {
        qWarning() << __PRETTY_FUNCTION__ << " ID " << id << " was already used! New id is "
                   << maxId;
        returnId = maxId;
    }
    if (returnId == 0) {
        qCritical() << __PRETTY_FUNCTION__
                    << "This should never happen! ChannelName: " << channelName;
    }

    Channel::ChannelDefaults *cDefault = new Channel::ChannelDefaults;
    cDefault->min=channelMinVal;
    cDefault->max=channelMaxVal;
    cDefault->value=channelDefaultVal;

    m_outputChannels.insert(channelName, returnId);
    m_outputChannelDefaults.insert(returnId, cDefault);
    foreach (AgentInstance *agentInstance, m_agentInstances) {
        agentInstance->addOutputChannel(returnId, cDefault);
    }

    if (m_defaultAgentInstance)
        Q_ASSERT(m_outputChannels.count() == m_defaultAgentInstance->outputChannels().count());
    Q_ASSERT(m_outputChannels.count() == m_outputChannelDefaults.count());

    return returnId;
}

quint32 Agent::numInputChannels() const
{
    return m_inputChannels.count();
}

quint32 Agent::numOutputChannels() const
{
    return m_outputChannels.count();
}

QString Agent::inputChannelName(const BrainiacGlobals::BrainiacId id) const
{
    return m_inputChannels.key(id, QString("This should not appear!"));
}

QString Agent::outputChannelName(const BrainiacGlobals::BrainiacId id) const
{
    return m_outputChannels.key(id, QString("This should not appear!"));
}

Agent::~Agent() {
    foreach(Channel::ChannelDefaults* cDefault,m_inputChannelDefaults) {
        delete cDefault;
    }
    foreach(Channel::ChannelDefaults* cDefault,m_outputChannelDefaults) {
        delete cDefault;
    }
}
