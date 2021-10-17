

#include " anomaly_detection_util.h"

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
