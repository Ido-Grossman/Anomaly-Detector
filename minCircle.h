// ID: 324603422 (Santiago Szterenberg) && 208985424 (Ido Grossman)
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

Circle findMinCircle(Point** points,size_t size);
float distance(const Point& p1, const Point& p2);
bool isPointInside(const Circle& circle, const Point& point);
Circle MECFinder(Point** points, std::vector<Point> pointsOnCircle, ulong size);
Circle createCircleFromPoints(const Point& p1, const Point& p2);
bool isCircleValid(const Circle& circle, const std::vector<Point>& points);
Point getCenterOfCircle(double p1X, double p1Y, double p2X, double p2Y);
Circle createCircleFromPoints(const Point& p1, const Point& p2, const Point& p3);

#endif /* MINCIRCLE_H_ */