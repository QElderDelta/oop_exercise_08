#include <iostream>
#include <cmath>
#include <iomanip>

#include "point.hpp"

double calculateDistance(const Point& lhs, const Point& rhs) {
    return sqrt(pow(rhs.x - lhs.x, 2) + pow(rhs.y - lhs.y, 2));
}

double triangleSquare(const Point& p1, const Point& p2, const Point& p3) {
    return 0.5 * fabs((p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y));
}

bool operator<(const Point& lhs, const Point& rhs) {
    if(lhs.x != rhs.x) {
        return lhs.x < rhs.x;
    }
    return lhs.y < rhs.y;
}

std::istream& operator>>(std::istream& is, Point& p) {
    is >> p.x >> p.y;
    return is;
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << std::fixed << std::setprecision(3) << "[" << p.x << ", "  << p.y << "]";
    return os;
}
