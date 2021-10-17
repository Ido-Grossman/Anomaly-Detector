

#include " anomaly_detection_util.h"

<<<<<<< HEAD
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
=======
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
>>>>>>> 3c65d334fd19c7c3c970d4f9145a76f46f05e480
