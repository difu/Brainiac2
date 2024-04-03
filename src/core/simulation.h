#ifndef SIMULATION_H
#define SIMULATION_H

#include <QObject>

class Scene;

class Simulation : public QObject
{
    Q_OBJECT
public:
    explicit Simulation(QObject *parent = nullptr);

    void advance();
    quint32 currentFrame() const;
    void reset();
    quint32 startFrame() const;
    void setStartFrame(qint32 newStartFrame);

    quint32 endFrame() const;
    void setEndFrame(qint32 newEndFrame);

    void startSimulation();

    bool isRunning() const;

    qreal frameRate() const;
    void setFrameRate(qreal newFrameRate);

    virtual ~Simulation();

protected:
    void setCurrentFrame(quint32 newCurrentFrame);

private:
    quint32 m_startFrame;
    quint32 m_endFrame;
    quint32 m_currentFrame;

    qreal m_frameRate;

    bool m_isRunning;

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
