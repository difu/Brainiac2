#include "simulation.h"

#include "scene.h"

Simulation::Simulation(QObject *parent)
    : QObject{parent}
{
    m_scene = qobject_cast<Scene *>(parent);
    m_isRunning = false;
    setStartFrame(0);
    setEndFrame(24);
    setFrameRate(24.0f);
    reset();
}

void Simulation::advance()
{
    if (m_currentFrame == m_endFrame) {
        qInfo("End frame of simulation already reached: %d", m_endFrame);
        return;
    }
    quint32 nextFrame = m_currentFrame + 1;
    setCurrentFrame(nextFrame);
    emit advanced(m_currentFrame);
    if (m_currentFrame == m_endFrame) {
        m_isRunning = false;
        emit endFrameReached();
    }
}

quint32 Simulation::startFrame() const
{
    return m_startFrame;
}

void Simulation::setStartFrame(qint32 newStartFrame)
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

void Simulation::setEndFrame(qint32 newEndFrame)
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
}

bool Simulation::isRunning() const
{
    return m_isRunning;
}

void Simulation::startSimulation()
{
    m_isRunning = true;
}

Simulation::~Simulation() {}
