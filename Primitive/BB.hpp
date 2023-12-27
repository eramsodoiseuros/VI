//
//  AABB.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef BB_hpp
#define BB_hpp

#include "vector.hpp"
#include "ray.hpp"

typedef struct BB {
    Point min, max;
    void update (Point p) {
        if (p.X < min.X) min.X = p.X;
        else if (p.X > max.X) max.X = p.X;
        if (p.Y < min.Y) min.Y = p.Y;
        else if (p.Y > max.Y) max.Y = p.Y;
        if (p.Z < min.Z) min.Z = p.Z;
        else if (p.Z > max.Z) max.Z = p.Z;
    }

    bool intersect (Ray r) {
        Vector invRaydir = Vector(1/r.dir.X, 1/r.dir.Y, 1/r.dir.Z);
        float tminx, tmaxx, tymin, tymax, tzmin, tzmax;
        if (r.dir.X >= 0) {
            tminx = (min.X - r.o.X);
            tmaxx = (max.X - r.o.X);
        }
        else {
            tminx = (max.X - r.o.X);
            tmaxx = (min.X - r.o.X);
        }

        if (r.dir.Y >= 0) {
            tymin = (min.Y - r.o.Y) ;
            tymax = (max.Y - r.o.Y) ;
        }
        else {
            tymin = (max.Y - r.o.Y);
            tymax = (min.Y - r.o.Y);
        }

        if (r.dir.Z >= 0) {
            tzmin = (min.Z - r.o.Z);
            tzmax = (max.Z - r.o.Z);
        }
        else {
            tzmin = (max.Z - r.o.Z);
            tzmax = (min.Z - r.o.Z);
        }

        // min and max are minus origin
        Vector tLower = Vector(tminx * invRaydir.X, tymin * invRaydir.Y, tzmin * invRaydir.Z);
        Vector tUpper = Vector(tmaxx * invRaydir.X, tymax * invRaydir.Y, tzmax * invRaydir.Z);


        float tBoxMin = fmax(fmax(tLower.X, tLower.Y), tLower.Z);
        float tBoxMax = fmin(fmin(tUpper.X, tUpper.Y), tUpper.Z);

        return tBoxMin <= tBoxMax;
    }

} BB;

#endif /* AABB_hpp */
