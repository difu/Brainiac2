#include "simulation.h"
#include "scene.h"

#include <QTimerEvent>
#include <QMutexLocker>
#include <QDebug>

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
        qDebug() << "Advancing to frame " << m_currentFrame;
        quint32 nextFrame = m_currentFrame + 1;
        setCurrentFrame(nextFrame);
        emit advanced(m_currentFrame);
        if (m_currentFrame == m_endFrame) {
            m_isRunning = false;
            emit endFrameReached();
        }
        m_advanceMutex.unlock();
    }
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

Simulation::~Simulation() {}
