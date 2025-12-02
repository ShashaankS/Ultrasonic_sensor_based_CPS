#ifndef TANKFSM_H
#define TANKFSM_H

#include <Arduino.h>

enum TankState { FILL, HOLD, STOP };

class TankFSM {
public:
    TankFSM(int fillTh, int holdTh, int maxSafe);
    void update(float filteredDistance);
    TankState getState();
    const char* getStateName();

private:
    int fillThreshold, holdThreshold, maxSafe;
    TankState currentState;
};

#endif
