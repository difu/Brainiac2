#ifndef BRAINIACGLOBALS_H
#define BRAINIACGLOBALS_H

#include <qcolor.h>
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
        BOX,
        TUBE,
        GROUP,
        AGENT,
        ACTION,
        TRANSITION,
        UNKNOWN
    };
    enum RotTrans { RX=1, RY=2, RZ=3, TX=4, TY=5, TZ=6 };
    enum AnimationType { STATIC, LOCOMOTION, RAMP, TURNING, NOTYPE };

    enum Channels {
        CI_RX = 1,
        CI_RY = 2,
        CI_RZ = 3,
        CI_TX = 4,
        CI_TY = 5,
        CI_TZ = 6,
        CO_RX = 7,
        CO_RY = 8,
        CO_RZ = 9,
        CO_TX = 10,
        CO_TY = 11,
        CO_TZ = 12,
        CO_COLOR = 13 //!< color channel of the agent
    };

    static qreal grad2rad(qreal grad) { return grad*PI/180.0; }
    static qreal rad2grad(qreal rad) { return rad*180.0/PI; }

    static qreal sinGrad( qreal grad) { return sin(grad*PI/180.0); }
    static qreal cosGrad( qreal grad) { return cos(grad*PI/180.0); }

    static QColor BrainiacColor(qreal color);

    static qreal getRand(quint32 index);

    static const quint32 NUMBER_OF_RANDOMS = 300;
    static const qreal randoms[NUMBER_OF_RANDOMS];
};

#endif // BRAINIACGLOBALS_H
