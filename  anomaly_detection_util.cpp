//
// Created by ido on 17/10/2021.
//

#include " anomaly_detection_util.h"

float cov(float* x, float* y, int size){
    float tempArr[size];
    float xAverage = 0, yAverage = 0, xyAverage = 0;
    for(int i = 0; i < size; i++){
        // for x*y
        tempArr[i] = x[i] * y[i];
        // adding the sum to x and y
        xAverage += x[i];
        yAverage += y[i];
        xyAverage += tempArr[i];

    }
    xAverage /= size;
    yAverage /= size;
    xyAverage /= size;
    // E(xy) - E(x)E(y)
    return xyAverage - xAverage * yAverage;
    
}