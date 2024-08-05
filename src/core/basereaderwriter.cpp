#include "basereaderwriter.h"

BaseReaderWriter::BaseReaderWriter(QObject *parent)
    : QObject{parent}
{}

void BaseReaderWriter::checkUnknown(const ConfigBlock &confBlock) {
    if (confBlock.type != BaseReaderWriter::UNKNOWN) {
        qCritical() << "Parsing error!";
    }
}

void BaseReaderWriter::clearConfigBlock(ConfigBlock &confBlock) {
    confBlock.lines.clear();
    confBlock.type = BaseReaderWriter::UNKNOWN;
}
