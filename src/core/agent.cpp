#include "agent.h"
#include "agentinstance.h"
#include "agentreaderwriter.h"
#include "brain/brain.h"

#include <QtDebug>
#include <QtGlobal>

#include "scene.h"
#include "body/body.h"

Agent::Agent(Scene *parent)
    : QObject{parent} {
    m_scene = parent;
    m_scene->addAgent(this);
    m_defaultAgentInstance = nullptr;

    m_agentReaderWriter = new AgentReaderWriter(this);
    m_body = new Body(this);
    m_brain = new Brain(this);
    constexpr Channel::ChannelDefaults trans_defaults{.min = -100.0, .max = 100.0, .value = 0.0};

    constexpr Channel::ChannelDefaults rot_defaults{.min = -180.0, .max = 180.0, .value = 0.0};

    addOutputChannel(QString("tx"), trans_defaults.min, trans_defaults.max, trans_defaults.value,
                     BrainiacGlobals::CO_TX);
    addOutputChannel(QString("ty"), trans_defaults.min, trans_defaults.max, trans_defaults.value,
                     BrainiacGlobals::CO_TY);
    addOutputChannel(QString("tz"), trans_defaults.min, trans_defaults.max, trans_defaults.value,
                     BrainiacGlobals::CO_TZ);

    addOutputChannel(QString("rx"), rot_defaults.min, rot_defaults.max, rot_defaults.value, BrainiacGlobals::CO_RX);
    addOutputChannel(QString("ry"), rot_defaults.min, rot_defaults.max, rot_defaults.value, BrainiacGlobals::CO_RY);
    addOutputChannel(QString("rz"), rot_defaults.min, rot_defaults.max, rot_defaults.value, BrainiacGlobals::CO_RZ);

    addInputChannel(QString("tx"), trans_defaults.min, trans_defaults.max, trans_defaults.value,
                    BrainiacGlobals::CI_TX);
    addInputChannel(QString("ty"), trans_defaults.min, trans_defaults.max, trans_defaults.value,
                    BrainiacGlobals::CI_TY);
    addInputChannel(QString("tz"),
                    trans_defaults.min,
                    trans_defaults.max,
                    trans_defaults.value,
                    BrainiacGlobals::CI_TZ);

    addInputChannel(QString("rx"),
                    rot_defaults.min,
                    rot_defaults.max,
                    rot_defaults.value,
                    BrainiacGlobals::CI_RX);
    addInputChannel(QString("ry"),
                    rot_defaults.min,
                    rot_defaults.max,
                    rot_defaults.value,
                    BrainiacGlobals::CI_RY);
    addInputChannel(QString("rz"),
                    rot_defaults.min,
                    rot_defaults.max,
                    rot_defaults.value,
                    BrainiacGlobals::CI_RZ);

    addOutputChannel(QString("color"), 0.0, 1.0, 1.0, BrainiacGlobals::CO_COLOR);
}

AgentInstance *Agent::addAgentInstance(Locator *locator) {
    auto *newAgentInstance = new AgentInstance(locator, this);
    m_agentInstances.append(newAgentInstance);
    this->scene()->addAgentInstance(newAgentInstance);
    qDebug()  << "Added new AgentInstance to Scene";
    return newAgentInstance;
}

QString Agent::name() const {
    return objectName();
}

bool Agent::setName(const QString &newName) {
    if (objectName() == newName) {
        return true;
    }
    foreach(auto *agent, m_scene->agents()) {
        if (agent == this) {
            continue;
        }
        if (agent->name() == newName) {
            return false;
        }
    }

    setObjectName(newName);
    return true;
}

AgentInstance *Agent::defaultAgentInstance() const {
    return m_defaultAgentInstance;
}

void Agent::setDefaultAgentInstance(AgentInstance *newDefaultAgentInstance) {
    m_defaultAgentInstance = newDefaultAgentInstance;
}

Body *Agent::body() const {
    return m_body;
}

Brain *Agent::brain() const {
    return m_brain;
}

QList<AgentInstance *> Agent::agentInstances() const {
    return m_agentInstances;
}

QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults *> Agent::inputChannelDefaults() const {
    return m_inputChannelDefaults;
}

QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults *> Agent::outputChannelDefaults() const {
    return m_outputChannelDefaults;
}

QString Agent::fileName() const {
    return m_fileName;
}

bool Agent::load() {
    return false;
}

bool Agent::save() {
    bool ret = false;
    ret = m_agentReaderWriter->saveAsBAF();
    return ret;
}

QJsonObject Agent::toJson() const {
    QJsonObject obj;
    obj["name"] = objectName();
    return obj;
}

Scene *Agent::scene() const {
    return m_scene;
}

void Agent::dumpChannels() {
    qDebug() << "Input Channels:";
    foreach(BrainiacGlobals::BrainiacId id, m_inputChannels) {
        qDebug() << "Channel: " << m_inputChannels.key(id) << " ID: " << id;
    }
    qDebug() << "Output Channels:";
    foreach(BrainiacGlobals::BrainiacId id, m_outputChannels) {
        qDebug() << "Channel: " << m_outputChannels.key(id) << " ID: " << id;
    }
}

void Agent::setFileName(const QString &newFileName) {
    m_fileName = newFileName;
}

BrainiacGlobals::BrainiacId Agent::addInputChannel(const QString channelName,
                                                   qreal channelMinVal, qreal channelMaxVal, qreal channelDefaultVal,
                                                   BrainiacGlobals::BrainiacId id) {
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

    if (id == 0) {
        // requested a new ID
        returnId = maxId;
    }
    if (idFound) {
        qWarning() << __PRETTY_FUNCTION__ << " ID " << id << " for channel " << channelName
                << " was already used! New id is " << maxId;
        returnId = maxId;
    }
    if (returnId == 0) {
        qCritical() << __PRETTY_FUNCTION__
                 << "This should never happen! ChannelName: " << channelName;
    }

    auto *cDefault = new Channel::ChannelDefaults;
    cDefault->min = channelMinVal;
    cDefault->max = channelMaxVal;
    cDefault->value = channelDefaultVal;

    m_inputChannels.insert(channelName, returnId);
    m_inputChannelDefaults.insert(returnId, cDefault);
    foreach(AgentInstance *agentInstance, m_agentInstances) {
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
                                                    BrainiacGlobals::BrainiacId id) {
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

    if (id == 0) {
        // requested a new ID
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

    auto *cDefault = new Channel::ChannelDefaults;
    cDefault->min = channelMinVal;
    cDefault->max = channelMaxVal;
    cDefault->value = channelDefaultVal;

    m_outputChannels.insert(channelName, returnId);
    m_outputChannelDefaults.insert(returnId, cDefault);
    foreach(AgentInstance *agentInstance, m_agentInstances) {
        agentInstance->addOutputChannel(returnId, cDefault);
    }

    if (m_defaultAgentInstance)
        Q_ASSERT(m_outputChannels.count() == m_defaultAgentInstance->outputChannels().count());
    Q_ASSERT(m_outputChannels.count() == m_outputChannelDefaults.count());

    return returnId;
}

quint32 Agent::numInputChannels() const {
    return m_inputChannels.count();
}

quint32 Agent::numOutputChannels() const {
    return m_outputChannels.count();
}

QString Agent::inputChannelName(const BrainiacGlobals::BrainiacId id) const {
    return m_inputChannels.key(id, QString("This should not appear!"));
}

QString Agent::outputChannelName(const BrainiacGlobals::BrainiacId id) const {
    return m_outputChannels.key(id, QString("This should not appear!"));
}

Agent::~Agent() {
    foreach(Channel::ChannelDefaults* cDefault, m_inputChannelDefaults) {
        delete cDefault;
    }
    foreach(Channel::ChannelDefaults* cDefault, m_outputChannelDefaults) {
        delete cDefault;
    }
}

AgentReaderWriter *Agent::agentReaderWriter() const
{
    return m_agentReaderWriter;
}
