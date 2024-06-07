#include "simulation.h"
#include "scene.h"

#include "agentinstance.h"


#include <QtConcurrent/QtConcurrent>
#include <QDateTime>
#include <QDebug>
#include <QMutexLocker>
#include <QThread>
#include <QTimerEvent>

Simulation::Simulation(QObject *scene)
    : QObject{scene}
{
    m_scene = qobject_cast<Scene *>(scene);
    m_isRunning = false;
    m_timerId=0;
    setStartFrame(0);
    setEndFrame(24);
    setFrameRate(24.0f);
    reset();
}

void Simulation::advance()
{
    if (m_currentFrame >= m_endFrame) {
        qInfo("End frame of simulation already reached: %d", m_endFrame);
        return;
    }
    if(m_advanceMutex.tryLock()) {
        QDateTime startTime(QDateTime::currentDateTime());
        qDebug() << "Advancing to frame " << m_currentFrame;
        quint32 nextFrame = m_currentFrame + 1;
        setCurrentFrame(nextFrame);
        emit advanced(m_currentFrame);
        if (m_currentFrame == m_endFrame) {
            m_isRunning = false;
            emit endFrameReached();
        }
        // foreach (AgentInstance *agentInstance, m_scene->agentInstances()) {
        //     agentInstance->advance();
        // }
        QtConcurrent::blockingMap(m_scene->agentInstances(), &Simulation::advanceAgentInstance);
        // foreach(AgentInstance *agentInstance, m_scene->agentInstances()) {
        //     agentInstance->advanceCommit();
        // }
        QtConcurrent::blockingMap(m_scene->agentInstances(), &Simulation::advanceAgentInstanceCommit);
        m_advanceMutex.unlock();

        const QDateTime endTime(QDateTime::currentDateTime());
        const qint64 durationMS = startTime.msecsTo(endTime);
        const qint64 frameLengthMS = (qint64) ((1 / (m_frameRate / 1000.0)));
        const qint64 waitTimeMS = frameLengthMS - durationMS;
        if (waitTimeMS > 0) {
            qDebug() << "Current simTime " << currentSimulationTimeMS();
            qDebug() << "Frame took " << durationMS << "ms.";
            qDebug() << "Sleeping for " << waitTimeMS << "ms.";
            QThread::sleep(static_cast<std::chrono::nanoseconds>(waitTimeMS) * 1000);
        }
    }
}

void Simulation::advanceAgentInstance(AgentInstance *agentInstance) {
    agentInstance->advance();
}

void Simulation::advanceAgentInstanceCommit(AgentInstance *agentInstance) {
    agentInstance->advanceCommit();
}

qint64 Simulation::currentSimulationTimeMS() const
{
    return m_currentFrame * (qint64) ((1 / m_frameRate) * 1000.0);
}

quint32 Simulation::startFrame() const
{
    return m_startFrame;
}

void Simulation::setStartFrame(quint32 newStartFrame)
{
    if (m_startFrame == newStartFrame)
        return;
    m_startFrame = newStartFrame;
    emit startFrameChanged();
}

void Simulation::reset()
{
    setCurrentFrame(startFrame());
    foreach(AgentInstance *agentInstance, m_scene->agentInstances()) {
        agentInstance->reset();
    }
}

quint32 Simulation::endFrame() const
{
    return m_endFrame;
}

void Simulation::setEndFrame(quint32 newEndFrame)
{
    if (m_endFrame == newEndFrame)
        return;
    m_endFrame = newEndFrame;
    emit endFrameChanged();
}

quint32 Simulation::currentFrame() const
{
    return m_currentFrame;
}

void Simulation::setCurrentFrame(quint32 newCurrentFrame)
{
    if (m_currentFrame == newCurrentFrame)
        return;
    m_currentFrame = newCurrentFrame;
    emit currentFrameChanged();
}

qreal Simulation::frameRate() const
{
    return m_frameRate;
}

void Simulation::setFrameRate(qreal newFrameRate)
{
    m_frameRate = newFrameRate;
    this->killTimer(m_timerId);
    m_timerId=this->startTimer((1/m_frameRate)*1000);
}

bool Simulation::isRunning() const
{
    return m_isRunning;
}

void Simulation::startSimulation()
{
    m_isRunning = true;
}

void Simulation::timerEvent(QTimerEvent *event){
    if(event->timerId() == m_timerId && m_isRunning) {
        this->advance();
    }
}

Simulation::~Simulation() = default;
