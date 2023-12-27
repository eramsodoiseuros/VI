//
// Created by kiko on 12/05/2023.
//

#ifndef VI_RT_AREALIGHT_HPP
#define VI_RT_AREALIGHT_HPP

#include "light.hpp"
#include "RGB.hpp"
#include "vector.hpp"
#include "Triangle.hpp"

class AreaLight: public Light {
public:
    RGB intensity, power;
    Triangle *gem;
    float pdf;
    AreaLight (RGB _power, Point _v1, Point _v2, Point _v3, Vector _n): power(_power) {
        type = AREA_LIGHT;
        gem = new Triangle (_v1, _v2, _v3,_n);
        pdf = 1.f/gem->area(); // for uniform sampling the area
        intensity = _power * pdf;
    }
    ~AreaLight () {delete gem;}
    RGB Sample_L (float *r, Point *p, float &_pdf) {
        const float sqrt_r0 = sqrtf(r[0]);
        const float alpha = 1.f - sqrt_r0;
        const float beta = (1.f-r[1]) * sqrt_r0;
        const float gamma = r[1] * sqrt_r0;
        p->X = alpha*gem->v1.X + beta*gem->v2.X + gamma*gem->v3.X;
        p->Y = alpha*gem->v1.Y + beta*gem->v2.Y + gamma*gem->v3.Y;
        p->Z = alpha*gem->v1.Z + beta*gem->v2.Z + gamma*gem->v3.Z;
        _pdf = pdf;
        return intensity;
    }
};
#endif //VI_RT_AREALIGHT_HPP
