#pragma once

#include <array>

#include "figure.hpp"
#include "point.hpp"

class Rhombus : public Figure {
public:
    Rhombus(std::istream& is);
    Point Center() const override;
    double Square() const override;
    void Print(std::ostream& os) const override;
private:
    std::array<Point, 4> points;
    double smallerDiagonal, biggerDiagonal;
};

