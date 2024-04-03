#ifndef SCENE_H
#define SCENE_H

#include <QObject>

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

signals:
};

#endif // SCENE_H
