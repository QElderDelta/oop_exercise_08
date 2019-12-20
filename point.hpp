#pragma once

#include <iostream>

struct Point {
    double x, y;
};

double calculateDistance(const Point& lhs, const Point& rhs);
bool operator<(const Point& lhs, const Point& rhs);
std::istream& operator>>(std::istream& is, Point& p);
std::ostream& operator<<(std::ostream& os, const Point& p);
double triangleSquare(const Point& p1, const Point& p2, const Point& p3);
