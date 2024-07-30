#ifndef GENERATORPOINT_H
#define GENERATORPOINT_H

#include <QObject>
#include <QVector3D>
#include "generatorbase.h"

class GeneratorPoint : public GeneratorBase
{
    Q_OBJECT
public:
    explicit GeneratorPoint(QObject *parent = nullptr);
    void apply() override;
    [[nodiscard]] QVector3D centerPoint() const;
    void setCenterPoint(const QVector3D &newCenterPoint);

private:
    QVector3D m_centerPoint;
};

#endif // GENERATORPOINT_H
