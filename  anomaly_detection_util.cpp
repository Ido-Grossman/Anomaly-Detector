

#include " anomaly_detection_util.h"
#include <cmath>

// returns the covariance of X and Y
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

// returns the variance of X and Y
float var(float* x, int size) {
    float z = 0;
    // the mio of var(x)
    float mio = 0;
    for (int i = 0; i < size; i++) {
        mio += x[i];
        z += x[i] * x[i];
    }
    mio /= (float)size;
    mio *= mio;
    z /= (float)size;
    z -= mio;
    return z;
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
    return cov(x, y, size) / (sqrt(var(x, size) * sqrt(var(y, size))));
}
