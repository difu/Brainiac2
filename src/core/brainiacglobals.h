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

    enum ItemType {
        AND,
        OR,
        FUZZ,
        DEFUZZ,
        OUTPUT,
        INPUT,
        NOISE,
        TIMER,
        SPHERE,
        CUBE,
        TUBE,
        GROUP,
        AGENT,
        ACTION,
        TRANSITION
    };
    enum RotTrans { RX, RY, RZ, TX, TY, TZ };
    enum AnimationType { STATIC, LOCOMOTION, RAMP, TURNING, NOTYPE };
};

#endif // BRAINIACGLOBALS_H
