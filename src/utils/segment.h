#pragma once

// Line Segment class
// Used for raycasting

#include <SFML/Graphics.hpp>
#include <iostream>
#include "vec.h"
#include "rect.h"


template <typename Type>
class Segment {

private:
    Vector2<Type> begin_;
    Vector2<Type> end_;
public:
    Segment(Vector2<Type> begin, Vector2<Type> end): begin_(begin), end_(end) { }
    Segment(): begin_(Vector2<Type>()), end_(Vector2<Type>()) { }
    ~Segment() { }
    Vector2<Type> GetBegin() const { return begin_; }
    Vector2<Type> GetEnd() const { return end_; }
    void SetBegin(Vector2<Type> begin) { begin_ = begin; }
    void SetEnd(Vector2<Type> end) { end_ = end; }

    friend bool segmentComparator(Segment<Type> R1, Segment<Type> R2){
        Vector2<Type> A = R1.end_ - R1.begin_;
        Vector2<Type> B = R2.end_ - R2.begin_;
        if (A.getAngle() < B.getAngle()) return 0;
        if (A.getAngle() > B.getAngle()) return 1;
        else return 0;
    }
};
