#ifndef KALMANFILTER_H
#define KALMANFILTER_H

class KalmanFilter {
public:
    KalmanFilter(float Q, float R, float initialEstimate);
    float update(float measurement);

private:
    float Q, R, P, X;
};

#endif
