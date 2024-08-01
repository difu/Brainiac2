#ifndef BASEREADERWRITER_H
#define BASEREADERWRITER_H

#include <QObject>

class BaseReaderWriter : public QObject {
    Q_OBJECT

public:
    explicit BaseReaderWriter(QObject *parent = nullptr);

protected:
    inline static const QString _indent = "    ";
signals:
};

#endif // BASEREADERWRITER_H
