#include "TankFSM.h"

TankFSM::TankFSM(int fillTh, int holdTh, int maxSafeD) {
    fillThreshold = fillTh;
    holdThreshold = holdTh;
    maxSafe = maxSafeD;
    currentState = HOLD;
}

void TankFSM::update(float d) {

    if (d <= 5 || d > maxSafe) {
        currentState = STOP;
        return;
    }

    switch (currentState) {

    case FILL:
        if (d <= holdThreshold) currentState = HOLD;
        break;

    case HOLD:
        if (d > fillThreshold) currentState = FILL;
        break;

    case STOP:
        if (d > 0 && d < maxSafe) currentState = HOLD;
        break;
    }
}

TankState TankFSM::getState() {
    return currentState;
}

const char* TankFSM::getStateName() {
    switch (currentState) {
        case FILL: return "FILL";
        case HOLD: return "HOLD";
        case STOP: return "STOP";
    }
    return "UNKNOWN";
}
