#include "agent.h"
#include "agentinstance.h"
#include "brain/brain.h"

#include <QtDebug>
#include <QtGlobal>

Agent::Agent(QObject *parent)
    : QObject{parent}
{
    m_defaultAgentInstance = addAgentInstance();

    m_brain = new Brain(this);
    Channel::ChannelDefaults trans_defaults;
    trans_defaults.max = 100;
    trans_defaults.min = -100;
    trans_defaults.value = 0.0f;
    addOutputChannel("tx", trans_defaults, 1);
    addOutputChannel(QString("ty"), trans_defaults, 2);
    addOutputChannel(QString("tz"), trans_defaults, 3);

    Channel::ChannelDefaults rot_defaults;
    rot_defaults.max = 180.0f;
    rot_defaults.min = -180.0f;
    rot_defaults.value = 0.0f;
    addOutputChannel(QString("rx"), rot_defaults, 4);
    addOutputChannel(QString("ry"), rot_defaults, 5);
    addOutputChannel(QString("rz"), rot_defaults, 6);

    addInputChannel(QString("tx"), trans_defaults, 1);
    addInputChannel(QString("ty"), trans_defaults, 2);
    addInputChannel(QString("tz"), trans_defaults, 3);
}

AgentInstance *Agent::addAgentInstance()
{
    AgentInstance *newAgentInstance = new AgentInstance(this);
    m_agentInstances.append(newAgentInstance);
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

QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults> Agent::inputChannelDefaults() const
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

void Agent::setFileName(const QString &newFileName)
{
    m_fileName = newFileName;
}

BrainiacGlobals::BrainiacId Agent::addInputChannel(const QString channelName,
                                                   Channel::ChannelDefaults defaults,
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
    m_inputChannels.insert(channelName, returnId);
    m_inputChannelDefaults.insert(returnId, defaults);
    foreach (AgentInstance *agentInstance, m_agentInstances) {
        agentInstance->addInputChannel(returnId, &defaults);
    }
    if (m_defaultAgentInstance) {
        Q_ASSERT(m_inputChannels.count() == m_defaultAgentInstance->inputChannels().count());
    }
    Q_ASSERT(m_inputChannels.count() == m_inputChannelDefaults.count());
    return returnId;
}

BrainiacGlobals::BrainiacId Agent::addOutputChannel(const QString channelName,
                                                    Channel::ChannelDefaults defaults,
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
    m_outputChannels.insert(channelName, returnId);
    m_outputChannelDefaults.insert(returnId, defaults);
    foreach (AgentInstance *agentInstance, m_agentInstances) {
        agentInstance->addOutputChannel(returnId, &defaults);
    }
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
