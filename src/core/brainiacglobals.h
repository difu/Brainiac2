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
        CI_RX, CI_RY, CI_RZ, CI_TX, CI_TY, CI_TZ,
        CO_RX, CO_RY, CO_RZ, CO_TX, CO_TY, CO_TZ,
        CO_COLOR //!< color channel of the agent
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
