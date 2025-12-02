#include "KalmanFilter.h"

KalmanFilter::KalmanFilter(float q, float r, float initialEstimate) {
    Q = q;
    R = r;
    P = 1;
    X = initialEstimate;
}

float KalmanFilter::update(float measurement) {
    P += Q;
    float K = P / (P + R);
    X = X + K * (measurement - X);
    P = (1 - K) * P;
    return X;
}
