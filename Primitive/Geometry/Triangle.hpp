//
// Created by kiko on 12/05/2023.
//

#ifndef VI_RT_TRIANGLE_HPP
#define VI_RT_TRIANGLE_HPP


#include <cmath>

#include "vector.hpp"
#include "BB.hpp"
#include "intersection.hpp"
#include "geometry.hpp"

class Triangle: public Geometry {
public:
    Point v1, v2, v3;
    Vector normal,edge1, edge2;
    BB bb;

    Triangle(Point _v1, Point _v2, Point _v3, Vector _normal):
            v1(_v1), v2(_v2), v3(_v3), normal(_normal) {
        edge1 = v1.vec2point(v2); edge2 = v1.vec2point(v3);
        bb.min.set(v1.X, v1.Y, v1.Z); bb.max.set(v1.X, v1.Y, v1.Z);
        bb.update(v2); bb.update(v3);
    }
    double area() const {
        Vector cross = edge1.cross(edge2);
        double length = std::sqrt(cross.X * cross.X + cross.Y * cross.Y + cross.Z * cross.Z);
        double area = 0.5 * length;
        return area;
    }
    bool intersect(Ray r, Intersection *isect) {
        const float EPSILON_ = 0.0000001;
        Vector e1, e2, h, s, q;
        float a,f,u,v;
        e1 = v2.vec2point(v1); // v2 - v1
        e2 = v3.vec2point(v1); // v3 - v1
        h = r.dir.cross(e2);
        a = e1.dot(h);
        if (a > -EPSILON_ && a < EPSILON_)
            return false;
        f = 1.0/a;
        s = r.o.vec2point(v1); // r.o - v1
        u = f * s.dot(h);
        if (u < 0.0 || u > 1.0)
            return false;
        q = s.cross(e1);
        v = f * r.dir.dot(q);
        if (v < 0.0 || u + v > 1.0)
            return false;
        // At this stage we can compute t to find out where the intersection point is on the line.
        float t = f * e2.dot(q);
        if (t > EPSILON_) // ray intersection
        {
            isect->p = r.o + r.dir * t;
            isect->gn = normal;
            isect->sn = normal;
            isect->wo = -r.dir;
            isect->depth = t;
            return true;
        }
        else // This means that there is a line intersection but not a ray intersection.
            return false;
    }
};


#endif //VI_RT_TRIANGLE_HPP
