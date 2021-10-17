

#include " anomaly_detection_util.h"
#include <math.h>

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
    mio /= size;
    mio *= mio;
    z /= size;
    z -= mio;
    return z;
}

// performs a linear regression and return s the line equation
Line linear_reg(Point** points, int size) {
    float x[size];
    float y[size];
    for (int i = 0; i < size; ++i) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    float a = cov(x,y, size) / var(x, size);
    float xAve = 0;
    float yAve = 0;
    for (int i = 0; i < size; ++i) {
        xAve += x[i];
        yAve += y[i];
    }
    xAve /= size;
    yAve /= size;
    float b = yAve - (a * xAve);
    return Line(a, b);
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
    return cov(x, y, size) / (sqrt(var(x, size) * sqrt(var(y, size))));
}

// returns the deviation between point p and the line
float dev(Point p,Line l) {
    return abs(l.f(p.x) - p.y);
}
