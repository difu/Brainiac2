#ifndef BRAINIACGLOBALS_H
#define BRAINIACGLOBALS_H

#include <QtCore>

class BrainiacGlobals
{
public:
    static const qreal PI;
    static const qreal MAXQREAL;
    static const qreal MINQREAL;

    typedef quint32 BrainiacId;

    enum RotTrans { RX, RY, RZ, TX, TY, TZ };
    enum AnimationType { STATIC, LOCOMOTION, RAMP, TURNING, NOTYPE };
};

#endif // BRAINIACGLOBALS_H
