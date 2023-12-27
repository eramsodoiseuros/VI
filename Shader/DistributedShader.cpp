//
// Created by jafmalheiro on 12/05/2023.
//

#include <chrono>
#include <random>
#include <omp.h>
#include "DistributedShader.hpp"
#include "AreaLight.hpp"
#include "PointLight.hpp"

RGB DistributedShader::areaLight(Intersection isect, Phong* f, Light* l, RGB color, std::uniform_real_distribution<float> distribution, std::default_random_engine generator){
    int num_samples = 100;
    RGB accumulated_color(0., 0., 0.);

    if (!f->Kd.isZero()) {
        RGB L, Kd = f->Kd;
        Point lpoint;
        float l_pdf;
        auto *al = dynamic_cast<AreaLight*>(l);

        for (int i = 0; i < num_samples; ++i) {
            float rnd[2];
            rnd[0] = distribution(generator);
            rnd[1] = distribution(generator);
            L = al->Sample_L(rnd, &lpoint, l_pdf);

            Vector Ldir = isect.p.vec2point(lpoint);
            const float Ldistance = Ldir.norm();
            Ldir.normalize();

            float cosL = Ldir.dot(isect.sn);
            float cosL_LA = Ldir.dot(al->gem->normal);

            if (cosL>0. and cosL_LA<=0.) {
                Ray shadow(isect.p, Ldir);
                shadow.adjustOrigin(isect.gn);

                if (scene->visibility(shadow, Ldistance-EPSILON)) {
                    accumulated_color += (Kd * L * cosL) / l_pdf;
                }
            }
        }
    }

    color += accumulated_color / num_samples;
    return color;
}


RGB DistributedShader::pointLight(Intersection isect, Phong* f, Light* l, RGB color) {
    if (!f->Kd.isZero()) {
        Point lpoint;
        // get the position and radiance of the light source
        RGB L = (l)->Sample_L(nullptr, &lpoint);

        // compute the direction from the intersection to the light
        Vector Ldir = isect.p.vec2point(lpoint);

        // Compute the distance between the intersection and the light source
        const float Ldistance = Ldir.norm();

        Ldir.normalize(); // now normalize Ldir

        // compute the cosine (Ldir , shading normal)
        float cosL = Ldir.dot(isect.sn);

        if (cosL>0.) { // the light is NOT behind the primitive

            // generate the shadow ray
            Ray shadow(isect.p, Ldir);

            // adjust origin EPSILON along the normal: avoid self occlusion
            shadow.adjustOrigin(isect.gn);

            if (scene->visibility(shadow, Ldistance-EPSILON)) // light source not occluded
                color += f->Kd * L * cosL;
        } // end cosL > 0.
        // the light is behind the primitive
    }
    return color;
}


RGB DistributedShader::directLighting(Intersection isect, Phong* f) {
    RGB color(0.,0.,0.);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<float> distribution(0.0, 1.0);

    // Seleciona aleatoriamente uma luz
    int chosen_idx = (int)(floor(distribution(generator) * scene->numLights));
    Light* chosen_light = scene->lights[chosen_idx];

    if (chosen_light->type == AMBIENT_LIGHT) {
        if (!f->Ka.isZero()) {
            RGB Ka = f->Ka;
            color += Ka * chosen_light->L();
        }
    } else if (chosen_light->type == POINT_LIGHT) {
        color = pointLight(isect, f, chosen_light, color);
    } else if (chosen_light->type == AREA_LIGHT) {
        color = areaLight(isect, f, chosen_light, color, distribution, generator);
    }

    return color;
}
RGB DistributedShader::specularReflection(Intersection isect, Phong *f){
    // generate the specular ray
    float cos = isect.gn.dot(isect.wo);
    Vector Rdir = 2.f * cos * isect.gn - isect.wo;
    Ray specular(isect.p, Rdir);
    specular.adjustOrigin(isect.gn);
    Intersection s_isect;

    // trace ray
    bool intersected = scene->trace(specular, &s_isect);

    // shade this intersection
    RGB color = shade(intersected, s_isect, 0);

    return color;
}

RGB DistributedShader::shade(bool intersected, Intersection isect, int depth) {
    RGB color(0.,0.,0.);
    if (!intersected) return (background);

    // intersection with a light source
    if (isect.isLight) return isect.Le;
    Phong *f = (Phong *)isect.f;

    // if there is a specular component sample it
    if (!f->Ks.isZero()) color += specularReflection (isect, f);

    // if there is a diffuse component do direct light
    if (!f->Kd.isZero()) color += directLighting(isect, f);

    float gamma = 2.2;
    color.R = pow(color.R, 1.0 / gamma);
    color.G = pow(color.G, 1.0 / gamma);
    color.B = pow(color.B, 1.0 / gamma);

    return color;
}

