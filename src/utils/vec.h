/**
 * Vector classes for vector math
*/

#pragma once

#include <iostream>
#include <cmath>

template <typename Type>
struct Vec2 {
public:
    Type x_, y_;
    Vec2(): x_(0.0), y_(0.0) { }
    Vec2(Type x, Type y): x_(x), y_(y) { }
};


template <typename Type>
class Vector2: public Vec2<Type> {

public:

    // Constructors
    Vector2(Type x, Type y): Vec2<Type>(x, y) { }
    Vector2(const Vector2<Type> &constructorVector): Vec2<Type>(constructorVector.GetX(), constructorVector.GetY()) { }
    Vector2(): Vec2<Type>() { }

    // Destructor
    ~Vector2() { }

    // Accessor methods
    Type GetX(void) const { return this->x_; }
    Type GetY(void) const { return this->y_; }

    void SetX(Type x) { this->x_ = x; }
    void SetY(Type y) { this->y_ = y; }

    // Vector addition
    Vector2 operator+(const Vector2 &vec) const { 
        return Vector2(this->x_ + vec.GetX(), this->y_ + vec.GetY()); 
    }

    // Vector subtraction
    Vector2 operator-(const Vector2 &vec) const {
        return Vector2(this->x_ - vec.x_, this->y_ - vec.y_);
    }

    // Squared magnitude of the vector
    Type SquaredMagnitude() const {
        return this->x_ * this->x_ + this->y_ * this->y_;
    }

    // Vector subtraction without creating a new vector
    Vector2& operator-=(const Vector2 &vec) {
        this->x_ -= vec.GetX();
        this->y_ -= vec.GetY();
        return *this;
    }

    Vector2 operator+=(const Vector2 &vec) {
        this->x_ += vec.GetX();
        this->y_ += vec.GetY();
        return *this;
    }

    Vector2 operator/= (Type scalar) {
        if (scalar != 0) {
            this->x_ /= scalar;
            this->y_ /= scalar;
        }
        return *this;
    }

    // Dot product
    Type Dot(const Vector2<Type> &vec) const { 
        return this->x_ * vec.GetX() + this->y_ * vec.GetY(); 
    }

    // Magnitude of the vector
    Type Magnitude() const {
        return std::sqrt(this->x_ * this->x_ + this->y_ * this->y_);
    }

    // Normalize the vector
    Vector2 Normalize() const {
        Type mag = Magnitude();
        if (mag > 0) {
            return Vector2(this->x_ / mag, this->y_ / mag);
        }
        return *this;
    }

    // Scalar multiplication
    Vector2 operator*(Type scalar) const {
        return Vector2(this->x_ * scalar, this->y_ * scalar);
    }

    // Scalar division
    Vector2 operator/(Type scalar) const {
        if (scalar != 0) {
            return Vector2(this->x_ / scalar, this->y_ / scalar);
        }
        return *this;
    }

    Type cross(const Vector2<Type>& other) const {
        return this->x_ * other.GetY() - this->y_ * other.GetX();
    }

    Type distanceTo(const Vector2<Type>& other) const {
        Type dx = this->x_ - other.GetX();
        Type dy = this->y_ - other.GetY();
        return sqrt(dx * dx + dy * dy);
    }

    // Output stream overload
    friend std::ostream& operator<<(std::ostream &os, const Vector2<Type> &vec) {
        os << vec.GetX() << ", " << vec.GetY();
        return os;
    }
};

template <typename Type>
std::ostream& operator<<(std::ostream &os, const Vector2<Type> &vec) {
    os << vec.GetX() << ", " << vec.GetY();
    return os;
}
