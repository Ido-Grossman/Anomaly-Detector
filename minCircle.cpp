// ID: 324603422 (Santiago Szterenberg) && 208985424 (Ido Grossman).
#include "anomaly_detection_util.h"
#include "minCircle.h"

bool isPointInside(const Circle& circle, const Point& point){
    double radius = circle.radius;
    if(distance(circle.center, point) <= radius)
        return true;
    return false;
}

Circle minimumCircle(std::vector<Point> points){
    ulong size = points.size();
    if (size == 0)
        return {Point(0,0), 0};
    else if (size == 1)
        return {points[0], 0};
    else if (size == 2)
        return createCircleFromPoints(points[0], points[1]);
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            Circle circle = createCircleFromPoints(points[i], points[j]);
            if (isCircleValid(circle, points))
                return circle;
        }
    }
    return createCircleFromPoints(points[0], points[1], points[2]);
}

float distance(const Point& p1, const Point& p2){
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

Circle findMinCircle(Point** points, size_t size){
    std::vector<Point> pointsCopy = {};
for (int i = 0; i < size; ++i) {
        pointsCopy.push_back(*points[i]);
    }
    std::vector<Point> pointsOnCircle = {};
    return MECFinder(pointsCopy, pointsOnCircle, size);
}

Circle MECFinder(std::vector<Point>& points, std::vector<Point> pointsOnCircle, ulong size){
    if (size == 0 || pointsOnCircle.size() == 3)
        return minimumCircle(pointsOnCircle);
    int random = rand() % size;
    Point point = points[random];
    std::swap(points[random], points[size - 1]);
    Circle circle = MECFinder(points, pointsOnCircle, size - 1);
    if (isPointInside(circle, point)) {
        return circle;
    }
    pointsOnCircle.push_back(point);
    // Return the MEC for P - {point} and R U {point}
    return MECFinder(points, pointsOnCircle, size - 1);
}

Circle createCircleFromPoints(const Point& p1, const Point& p2, const Point& p3){
    Point point = getCenterOfCircle(p2.x - p1.x, p2.y - p1.y, p3.x - p1.x, p3.y - p1.y);
    point.x += p1.x;
    point.y += p1.y;
    return {point, distance(point, p1)};
}

Circle createCircleFromPoints(const Point& p1, const Point& p2){
    Point center = Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2 );
    return {center, distance(p1, p2) / 2 };
}

bool isCircleValid(const Circle& circle, const std::vector<Point>& points){
    for(const Point& point : points)
        if (!isPointInside(circle, point))
            return false;
    return true;
}
Point getCenterOfCircle(double p1X, double p1Y, double p2X, double p2Y){
    double xCenter = p1X * p1X + p1Y * p1Y;
    double yCenter = p2X * p2X + p2Y * p2Y;
    double slope = p1X * p2Y - p1Y * p2X;
    return Point((p2Y * xCenter - p1Y * yCenter) / (2 * slope),(p1X * yCenter - p2X * xCenter) / (2 * slope) );
}