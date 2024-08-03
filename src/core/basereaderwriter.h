#ifndef BASEREADERWRITER_H
#define BASEREADERWRITER_H

#include <QObject>

#include "brainiacglobals.h"

class BaseReaderWriter : public QObject {
    Q_OBJECT

public:
    enum ConfigBlockType { UNKNOWN, FUZZY, SEGMENT, CONNECTIONS, GENERATOR, AGENT };

    struct ConfigBlock {
        ConfigBlockType type = BaseReaderWriter::UNKNOWN;
        BrainiacGlobals::ItemType itemType = BrainiacGlobals::UNKNOWN;
        QStringList lines;
    };

    explicit BaseReaderWriter(QObject *parent = nullptr);

    static void clearConfigBlock(ConfigBlock &confBlock);

protected:
    inline static const QString _indent = "    ";
signals:
};

#endif // BASEREADERWRITER_H
