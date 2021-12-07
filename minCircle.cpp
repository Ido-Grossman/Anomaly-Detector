// ID: 324603422 (Santiago Szterenberg) && 208985424 (Ido Grossman).
#include "anomaly_detection_util.h"
#include "minCircle.h"

/*
 * takes a point and a circle and checks if the point is inside the circle, return true if it is, else false.
 */
bool isPointInside(const Circle& circle, const Point& point){
    double radius = circle.radius;
    if(distance(circle.center, point) <= radius)
        return true;
    return false;
}

/*
 * given an array of max size 3, creates the minimum circle from those points.
 */
Circle minimumCircle(std::vector<Point> points){
    ulong size = points.size();
    // If the size of the points is 0, there is no valid minimum circle.
    if (size == 0)
        return {Point(0,0), 0};
    // if the size is 1 it returns the circle that his center is the point and the radius is 0.
    else if (size == 1)
        return {points[0], 0};
    // If the size is 2 it creates a circle from this 2 points.
    else if (size == 2)
        return createCircleFromPoints(points[0], points[1]);
    // If the size is 3 it goes over each pair and check if it can create a circle from this pair and return it.
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            Circle circle = createCircleFromPoints(points[i], points[j]);
            if (isCircleValid(circle, points))
                return circle;
        }
    }
    // If it can't create a circle from all the pairs, it creates a circle from all 3 of the points.
    return createCircleFromPoints(points[0], points[1], points[2]);
}

// Given 2 points it returns the distance between those 2 points.
float distance(const Point& p1, const Point& p2){
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

// Given an array of points and its size, it finds the minimum circle that encloses those points and returns it.
Circle findMinCircle(Point** points, size_t size){
    // Creates a copy of the points in a vector and activates the MECFinder function on the vector and an empty vector
    std::vector<Point> pointsOnCircle = {};
    return MECFinder(points, pointsOnCircle, size);
}

/*
 * Given a vector of points and a vector of points on the circle and the size of the circle, it finds the MEC of those
 * points.
 */
Circle MECFinder(Point** points, std::vector<Point> pointsOnCircle, ulong size){
    // if we have no more points or the size of points on circle is 3, we can create a minimum circle from the points
    // on the circle.
    if (size == 0 || pointsOnCircle.size() == 3)
        return minimumCircle(pointsOnCircle);
    // Gets a random value and sets the mod size of him inside random parameter.
    int random = rand() % size;
    // takes the point in the random index and swap it with the last point in the vector.
    Point point = *points[random];
    std::swap(points[random], points[size - 1]);
    // Recursively activates the function in order to find the MEC by calling it with size - 1 so we "remove" the
    // random point.
    Circle circle = MECFinder(points, pointsOnCircle, size - 1);
    // If all the points are inside the given circle from the recursive calls, it returns the circle.
    if (isPointInside(circle, point)) {
        return circle;
    }
    // If not all the points are inside the circle, it means the random point is on the circle so it adds it to the
    // pointsOnCircle vector
    pointsOnCircle.push_back(point);
    // Calls recursively as before but now with 1 more point in the pointsOnCircle vector.
    return MECFinder(points, pointsOnCircle, size - 1);
}

/*
 * Given 3 points, creates a circle from those points.
 */
Circle createCircleFromPoints(const Point& p1, const Point& p2, const Point& p3){
    Point point = getCenterOfCircle(p2.x - p1.x, p2.y - p1.y, p3.x - p1.x, p3.y - p1.y);
    point.x += p1.x;
    point.y += p1.y;
    return {point, distance(point, p1)};
}

/*
 * Given 2 points, it creates a circle from those 2 points.
 */
Circle createCircleFromPoints(const Point& p1, const Point& p2){
    Point center = Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2 );
    return {center, distance(p1, p2) / 2 };
}

/*
 * Given a circle and a vector of points, it checks if all the points are inside the circle and if they are it returns
 * True, else it returns false.
 */
bool isCircleValid(const Circle& circle, const std::vector<Point>& points){
    for(const Point& point : points)
        if (!isPointInside(circle, point))
            return false;
    return true;
}

/*
 * Given 2 points it returns the center of the circle based on those 2 points.
 */
Point getCenterOfCircle(double p1X, double p1Y, double p2X, double p2Y){
    double xCenter = p1X * p1X + p1Y * p1Y;
    double yCenter = p2X * p2X + p2Y * p2Y;
    double slope = p1X * p2Y - p1Y * p2X;
    return Point((p2Y * xCenter - p1Y * yCenter) / (2 * slope),(p1X * yCenter - p2X * xCenter) / (2 * slope) );
}