//
// Created by jafmalheiro on 23/03/2023.
//


#include "AmbientShader.hpp"
#include "Phong.hpp"

RGB AmbientShader::shade(bool intersected, Intersection isect, int depth) {
    RGB color(0.,0.,0.);

    // if no intersection, return background
    if (!intersected) {
        return (background);
    }

    // verify whether the intersected object has an ambient component
    Phong *f = (Phong *)isect.f;
    if (f->Ka.isZero()) return color;
    RGB Ka = f->Ka;

    // ambient shade
    // Loop over scene's light sources and process Ambient Lights
    for (auto light_itr = scene->lights.begin() ; light_itr != scene->lights.end() ; light_itr++) {

        if ((*light_itr)->type == AMBIENT_LIGHT) {  // is it an ambient light ?
            color += Ka * (*light_itr)->L();
            continue;
        }
    }

    return color;
};