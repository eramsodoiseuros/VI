//
//  Ray.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef Ray_hpp
#define Ray_hpp

#include "vector.hpp"

const float EPSILON=1e-3;

class Ray {
public:
    Point o; // ray origin
    Vector dir; // ray direction
    int x,y;  // pixel associated with this pixel
    Ray () {}
    Ray (Point o, Vector d): o(o),dir(d) {}
    ~Ray() {}
    void adjustOrigin (Vector normal) {
        Vector offset = EPSILON * normal;
        if (dir.dot(normal) < 0)
            offset = -1.f * offset;
        o.X = o.X + offset.X;
        o.Y = o.Y + offset.Y;
        o.Z = o.Z + offset.Z;
    }
};

#endif /* Ray_hpp */
