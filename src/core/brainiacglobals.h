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
        TRANSITION,
        UNKNOWN
    };
    enum RotTrans { RX=1, RY=2, RZ=3, TX=4, TY=5, TZ=6 };
    enum AnimationType { STATIC, LOCOMOTION, RAMP, TURNING, NOTYPE };

    static qreal grad2rad(qreal grad) { return grad*PI/180.0; }
    static qreal rad2grad(qreal rad) { return rad*180.0/PI; }

    static qreal sinGrad( qreal grad) { return sin(grad*PI/180.0); }
    static qreal cosGrad( qreal grad) { return cos(grad*PI/180.0); }

    static qreal getRand(quint32 index)
    {
        return BrainiacGlobals::randoms[index % BrainiacGlobals::NUMBER_OF_RANDOMS];
    }

    static const quint32 NUMBER_OF_RANDOMS = 300;
    static const qreal randoms[NUMBER_OF_RANDOMS];
};

#endif // BRAINIACGLOBALS_H
