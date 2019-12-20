#include <cmath>

#include "hexagon.hpp"

Hexagon::Hexagon(std::istream& is) {
    is >> points[0] >> points[1] >> points[2] >> points[3] >> points[4] >> points[5];
}

Point Hexagon::Center() const {
    Point insideFigure{0, 0};
    Point result{0, 0};
    double square = this->Square();
    for(unsigned i = 0; i < points.size(); ++i) {
        insideFigure.x += points[i].x;
        insideFigure.y += points[i].y;
    }
    insideFigure.x /= points.size();
    insideFigure.y /= points.size();
    for(unsigned i = 0; i < points.size(); ++i) {
        double tempSquare = triangleSquare(points[i], points[(i + 1) % points.size()], 
                insideFigure); 
        result.x += tempSquare * (points[i].x + points[(i + 1) % points.size()].x
                + insideFigure.x) / 3.0;
        result.y += tempSquare * (points[i].y + points[(i + 1) % points.size()].y
                + insideFigure.y) / 3.0;
    }
    result.x /= square;
    result.y /= square;
    return result;
}

double Hexagon::Square() const {
    double result = 0;
    for(unsigned i = 0; i < points.size(); ++i) {
        Point p1 = i ? points[i - 1] : points[points.size() - 1];
        Point p2 = points[i];
        result += (p1.x - p2.x) * (p1.y + p2.y);
    }
    return fabs(result) / 2.0;
}

void Hexagon::Print(std::ostream& os) const {
    os << "Hexagon:";
    for(const auto& p : points) {
        os << p << ' ';
    }
    os << std::endl;
}
