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

    Vector2<Type> GetCenter() const {
        return Vector2<Type>((cornerA_.GetX() + cornerB_.GetX()) / 2, (cornerA_.GetY() + cornerB_.GetY()) / 2);
    }

    Vector2<Type> GetSize() const {
        return Vector2<Type>(GetWidth(), GetHeight());
    }

    Type GetWidth() const {
        return cornerB_.GetX() - cornerA_.GetX();
    }

    Type GetHeight() const {
        return cornerB_.GetY() - cornerA_.GetY();
    }

    void move(Vector2<Type> delta) {
        cornerA_ += delta;
        cornerB_ += delta;
    }

    void stretch(Vector2<double> factor){
        cornerA_ -= factor;
        cornerB_ += factor;
    }

    bool Contains(Vector2<Type> point) const {
        return (point.GetX() >= cornerA_.GetX() && point.GetX() <= cornerB_.GetX() && point.GetY() >= cornerA_.GetY() && point.GetY() <= cornerB_.GetY());
    }

   bool Intersects(const Rect<Type>& rect) const {
        if (cornerA_.GetX() > rect.GetCornerB().GetX() || rect.GetCornerA().GetX() > cornerB_.GetX()) {
            return false;
        } if (cornerA_.GetY() > rect.GetCornerB().GetY() || rect.GetCornerA().GetY() > cornerB_.GetY()) {
            return false;
        } return true;
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

    Rect<Type> operator+(const Vector2<Type> &vec) const {
        return Rect<Type>(cornerA_ + vec, cornerB_ + vec);
    }

    friend std::ostream &operator<<(std::ostream &os, const Rect<Type> &rect) {
        os << "Corner A: " << rect.GetCornerA() << " Corner B: " << rect.GetCornerB();
        return os;
    }

};