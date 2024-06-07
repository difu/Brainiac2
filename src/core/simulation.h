#ifndef SIMULATION_H
#define SIMULATION_H
#include <QMutex>
#include <QObject>

class AgentInstance;
class Scene;

/**
 * @class Simulation
 * @brief The Simulation class represents a simulation of a scene.
 *
 * The Simulation class inherits QObject and provides functionality for advancing frames in the simulation,
 * setting start and end frames, managing the frame rate, and monitoring the simulation status.
 *
 * The simulation can be started by calling the startSimulation() function.
 * The current frame can be advanced using the advance() function.
 * The start and end frames can be set using the setStartFrame() and setEndFrame() functions.
 * The frame rate can be set using the setFrameRate() function.
 *
 * The setCurrentFrame() function is protected and used internally to set the current frame.
 * The timerEvent() function is also protected and handles the timer event for advancing frames.
 *
 * The class also provides signals startFrameChanged(), endFrameChanged(), endFrameReached(), and currentFrameChanged()
 * to notify the changes in the start frame, end frame, reaching the end frame, and current frame respectively.
 *
 * The class also provides properties startFrame, endFrame, and currentFrame for accessing and modifying the corresponding values.
 *
 * @note This class assumes the existence of a Scene class.
 */
class Simulation : public QObject
{
    Q_OBJECT
public:
    explicit Simulation(QObject *scene);

    void advance();

    static void advanceAgentInstance(AgentInstance *agentInstance);

    static void advanceAgentInstanceCommit(AgentInstance *agentInstance);

    [[nodiscard]] quint32 currentFrame() const;

    void reset();

    [[nodiscard]] quint32 startFrame() const;

    void setStartFrame(quint32 newStartFrame);

    [[nodiscard]] quint32 endFrame() const;

    void setEndFrame(quint32 newEndFrame);

    void startSimulation();

    [[nodiscard]] bool isRunning() const;

    [[nodiscard]] qreal frameRate() const;

    void setFrameRate(qreal newFrameRate);

    [[nodiscard]] qint64 currentSimulationTimeMS() const;

    ~Simulation() override;

protected:
    void setCurrentFrame(quint32 newCurrentFrame);
    virtual void timerEvent(QTimerEvent *event);

private:
    quint32 m_startFrame{};
    quint32 m_endFrame{};
    quint32 m_currentFrame{};

    qreal m_frameRate{};

    QMutex m_advanceMutex;
    bool m_isRunning;
    int m_timerId;

    Scene *m_scene;

    Q_PROPERTY(quint32 startFrame READ startFrame WRITE setStartFrame RESET reset NOTIFY
                   startFrameChanged FINAL)

    Q_PROPERTY(quint32 endFrame READ endFrame WRITE setEndFrame NOTIFY endFrameChanged FINAL)

    Q_PROPERTY(quint32 currentFrame READ currentFrame WRITE setCurrentFrame NOTIFY
                   currentFrameChanged FINAL)

signals:
    void advanced(quint32);
    void startFrameChanged();
    void endFrameChanged();
    void endFrameReached();
    void currentFrameChanged();
};

#endif // SIMULATION_H
