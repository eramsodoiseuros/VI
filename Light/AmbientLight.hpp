//
// Created by jafmalheiro on 23/03/2023.
//

#ifndef VI_RT_AMBIENTLIGHT_HPP
#define VI_RT_AMBIENTLIGHT_HPP

#include "light.hpp"

class AmbientLight: public Light {
public:
    RGB color;
    AmbientLight (RGB _color): color(_color) { type = AMBIENT_LIGHT; }
    ~AmbientLight () {}
    // return the Light RGB radiance for a given point : p
    RGB L (Point p) {return color;}
    // return the Light RGB radiance
    RGB L () {return color;}
    // return a point p and RGB radiance for a given probability pair prob[2]
    RGB Sample_L (float *prob, Point *p) {
        p = NULL;
        return color;
    }
};

#endif //VI_RT_AMBIENTLIGHT_HPP
