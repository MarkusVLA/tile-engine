#pragma once

#include <iostream>
#include <cmath>
#include "vec.h"

template <typename Type>

class Rect {

private:
    Vector2<Type> cornerA_;
    Vector2<Type> cornerB_;

public:

    Rect(Vector2<Type> cornerA, Vector2<Type> cornerB): cornerA_(cornerA), cornerB_(cornerB) { }
    Rect(): cornerA_(Vector2<Type>()), cornerB_(Vector2<Type>()) { }

    ~Rect() { }

    Vector2<Type> GetCornerA() const { return cornerA_; }
    Vector2<Type> GetCornerB() const { return cornerB_; }

    void SetCornerA(Vector2<Type> cornerA) { cornerA_ = cornerA; }
    void SetCornerB(Vector2<Type> cornerB) { cornerB_ = cornerB; }

    void move(Vector2<Type> delta) {
        cornerA_ += delta;
        cornerB_ += delta;
    }

    bool Contains(Vector2<Type> point) const {
        return (point.GetX() >= cornerA_.GetX() && point.GetX() <= cornerB_.GetX() && point.GetY() >= cornerA_.GetY() && point.GetY() <= cornerB_.GetY());
    }

    bool Intersects(const Rect<Type> &rect) const {
        return (Contains(rect.GetCornerA()) || Contains(rect.GetCornerB()));
    }

    bool Intersects(double x, double y) const {
        return (Contains(x, y));
    }

    bool Intersects(const Vector2<Type> &point) const {
        return (Contains(point));
    }

    bool operator==(const Rect<Type> &rect) const {
        return (cornerA_ == rect.GetCornerA() && cornerB_ == rect.GetCornerB());
    }

    bool operator!=(const Rect<Type> &rect) const {
        return (cornerA_ != rect.GetCornerA() || cornerB_ != rect.GetCornerB());
    }

    friend std::ostream &operator<<(std::ostream &os, const Rect<Type> &rect) {
        os << "Corner A: " << rect.GetCornerA() << " Corner B: " << rect.GetCornerB();
        return os;
    }

};