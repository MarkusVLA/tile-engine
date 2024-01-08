#pragma once

#include "vec.h"
#include "..\world\obstacle.h"

#include <limits>
#include <vector>

class Ray {
private:
    Vector2<double> start;
    Vector2<double> direction;

public:
    Ray(Vector2<double> start, float angle) : start(start) {
        direction = Vector2<double>(cos(angle), sin(angle));
    }

    Vector2<double> cast(const std::vector<Obstacle>& obstacles) {
        Vector2<double> closestPoint;
        float minDist = std::numeric_limits<float>::infinity();

        for (const auto& obstacle : obstacles) {
            Vector2<double> point = intersect(obstacle);
            float dist = start.distanceTo(point);

            if (dist < minDist) {
                minDist = dist;
                closestPoint = point;
            }
        }

        if (minDist == std::numeric_limits<float>::infinity()) {
            return start + direction * 1000.0f;
        }

        return closestPoint;
    }

private:

    Vector2<double> intersect(const Obstacle& obstacle) {
        Vector2<double> obstacleStart = obstacle.getStart();
        Vector2<double> obstacleEnd = obstacle.getEnd();
        Vector2<double> obstacleDirection = obstacleEnd - obstacleStart;

        double denominator = direction.cross(obstacleDirection);

        if (abs(denominator) < 1e-6) {
            return Vector2<double>(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
        }

        Vector2<double> diffStart = obstacleStart - start;
        double t1 = diffStart.cross(obstacleDirection) / denominator;
        double t2 = diffStart.cross(direction) / denominator;

        if (t1 >= 0.0f && t2 >= 0.0f && t2 <= 1.0f) {
            // Intersection point on the line segment
            return start + direction * t1;
        }

        // No valid intersection
        return Vector2<double>(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
    }
};
