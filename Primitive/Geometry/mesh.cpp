//
//  mesh.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 05/02/2023.
//

#include "mesh.hpp"
#include <float.h>

// see pbrt book (3rd ed.), sec 3.6.2, pag 157
bool Mesh::TriangleIntersect (Ray r, Face f, Intersection *isect) {

    if(!f.bb.intersect(r))
        return false;

    float EPSILON = 0.0000001;
    Point p0 = vertices[f.vert_ndx[0]];
    Point p1 = vertices[f.vert_ndx[1]];
    Point p2 = vertices[f.vert_ndx[2]];

    Vector edge1, edge2, wo;

    edge1 = Vector(p1.X - p0.X, p1.Y - p0.Y, p1.Z - p0.Z);
    edge2 = Vector(p2.X - p0.X, p2.Y - p0.Y, p2.Z - p0.Z);

    Vector h = r.dir.cross(edge2);

    float a = edge1.dot(h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.

    float factor = 1.0/a;
    Vector s = Vector(r.o.X - p0.X, r.o.Y - p0.Y, r.o.Z - p0.Z);
    float u = factor * s.dot(h);
    if (u < 0.0 || u > 1.0)
        return false;
    Vector q = s.cross(edge1);
    float v = factor * r.dir.dot(q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = factor * edge2.dot(q);
    if (t > EPSILON) // ray intersection
    {
        Point p = Point(r.o.X + r.dir.X * t, r.o.Y + r.dir.Y * t, r.o.Z + r.dir.Z * t);
        // set intersection info
        isect->p = p;
        isect->gn = f.geoNormal;
        isect->sn = f.geoNormal;

        wo = Vector(-r.dir.X, -r.dir.Y, -r.dir.Z);
        wo.normalize();

        isect->wo = wo;
        isect->depth = t;
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}

bool Mesh::intersect(Ray r, Intersection *isect) {
    bool intersect = false, intersect_this_face;
    Intersection min_isect, curr_isect;
    float min_depth = FLT_MAX;

    // Intersect the ray with the mesh BB
    if (!bb.intersect(r)) return false;

    // If it intersects, then loop through the faces
    for (auto & face : faces) {
        intersect_this_face = TriangleIntersect(r, face, &curr_isect);
        if (!intersect_this_face) continue;

        intersect = true;
        if (curr_isect.depth < min_depth) {  // this is closer
            min_depth = curr_isect.depth;
            min_isect = curr_isect;
        }
    }

    // If there was an intersection, update the intersection data
    if (intersect) {
        *isect = min_isect;
    }

    return intersect;
}
