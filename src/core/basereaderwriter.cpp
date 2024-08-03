#include "basereaderwriter.h"

BaseReaderWriter::BaseReaderWriter(QObject *parent)
    : QObject{parent}
{}


void BaseReaderWriter::clearConfigBlock(ConfigBlock &confBlock) {
    confBlock.lines.clear();
    confBlock.type = BaseReaderWriter::UNKNOWN;
}
