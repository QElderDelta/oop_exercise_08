#pragma once

#include<iostream>

#include "point.hpp"

enum class Figures {Rhombus, Pentagon, Hexagon};

class Figure {
public:
    virtual Point Center() const = 0;
    virtual double Square() const = 0;
    virtual void Print(std::ostream& os) const = 0;
    virtual ~Figure() = default;
};

