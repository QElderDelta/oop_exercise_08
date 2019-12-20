#pragma once

#include <iostream>
#include <array>

#include "figure.hpp"
#include "point.hpp"

class Pentagon : public Figure {
public:
    Pentagon(std::istream& is);
    Point Center() const override;
    double Square() const override;
    void Print(std::ostream& os) const override;
private:
    std::array<Point, 5> points;
};

