#ifndef GENERATORBASE_H
#define GENERATORBASE_H

#include <QList>
#include <QObject>

class Locator;
class Scene;

class GeneratorBase : public QObject
{
    Q_OBJECT
public:
    explicit GeneratorBase(QObject *parent);
    virtual void apply() = 0;

    [[nodiscard]] QList<Locator *> locators() const;

    Scene *scene();
    virtual ~GeneratorBase();

protected:
    QList<Locator *> m_locators;

private:
signals:
};

#endif // GENERATORBASE_H
