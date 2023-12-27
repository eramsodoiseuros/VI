//
//  AmbientShader.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include <cmath>
#include <random>
#include <chrono>
#include "WhittedShader.hpp"
#include "Phong.hpp"
#include "ray.hpp"
#include "PointLight.hpp"
#include "AreaLight.hpp"

RGB WhittedShader::directLighting (Intersection isect, Phong *f) {
    RGB color(0.,0.,0.);

    // Loop over scene's light sources
    for (auto l = scene->lights.begin() ; l != scene->lights.end() ; l++) {

        if ((*l)->type == AMBIENT_LIGHT) {  // is it an ambient light ?
            if (!f->Ka.isZero()) {
                RGB Ka = f->Ka;
                color += Ka * (*l)->L();
            }
            continue;
        }
        if ((*l)->type == AREA_LIGHT) {
            // Perform Monte Carlo sampling of the area light
            auto* areaLight = dynamic_cast<AreaLight*>(*l);

            /*
             * popular numbers are 16, 64 and 256
             * However, the optimal number of samples can depend on various factors
             * such as the complexity of the light source and the shading model.
            */
            int num_light_samples = 64;

            RGB accumulated_light(0.,0.,0.);

            for (int sample = 0; sample < num_light_samples; ++sample) {
                unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                std::default_random_engine generator(seed);
                std::uniform_real_distribution<float> distribution(0.0, 1.0);
                float r1 = distribution(generator);  // Generates a random float between 0 and 1
                float r2 = distribution(generator);  // Generate a random float between 0 and 1
                float r[2] = {r1, r2};

                Point sample_position;
                float light_pdf;
                RGB light_intensity = areaLight->Sample_L(r, &sample_position, light_pdf);

                Vector direction_to_light = sample_position.vec2point(isect.p);
                float distance_squared = direction_to_light.dot(direction_to_light);
                direction_to_light.normalize();

                // Adjust the origin point of the ray to avoid self-intersection
                Point adjusted_origin(isect.p.X + isect.gn.X * EPSILON, isect.p.Y + isect.gn.Y * EPSILON, isect.p.Z + isect.gn.Z * EPSILON);
                Ray shadow_ray(adjusted_origin, direction_to_light);

                bool lightVisible = scene->visibility(shadow_ray, std::sqrt(distance_squared) - EPSILON);

                // If the light sample is visible, calculate its contribution
                if (lightVisible) {
                    // The incident light direction is the normalized direction to the light
                    Vector wi = direction_to_light.normalized();
                    // The outgoing direction is the direction of the outgoing ray
                    Vector wo = -isect.wo;
                    RGB brdf_val = f->f(wi, wo);
                    // Calculate the cosine of the angle between the incident light direction and the surface normal
                    float cos_theta = std::max(0.f, direction_to_light.dot(isect.sn));
                    // Calculate the contribution of the light sample to the accumulated light
                    accumulated_light += brdf_val * cos_theta * (light_intensity / (distance_squared * light_pdf));
                }
            }

            RGB final_light = accumulated_light / float(num_light_samples);
            color = color + final_light;
        }

        if ((*l)->type == POINT_LIGHT) {  // is it a point light ?
            if (!f->Kd.isZero()) {
                Point lpoint;
                // get the position and radiance of the light source
                RGB L = (*l)->Sample_L(NULL, &lpoint);

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
        }
    }
    return color;
}

RGB WhittedShader::specularReflection (Intersection isect, Phong *f, int depth) {

    // generate the specular ray
    float cos = isect.gn.dot(isect.wo);
    Vector Rdir = 2.f * cos * isect.gn - isect.wo;
    Ray specular(isect.p, Rdir);
    specular.adjustOrigin(isect.gn);
    Intersection s_isect;

    // trace ray
    bool intersected = scene->trace(specular, &s_isect);

    // shade this intersection
    RGB color = shade(intersected, s_isect, depth);

    return color;
}

RGB WhittedShader::shade(bool intersected, Intersection isect, int depth) {
    RGB color(0.,0.,0.);

    // if no intersection, return background
    if (!intersected) {
        return (background);
    }

    // get the BRDF
    Phong *f = (Phong *)isect.f;

    // if there is a specular component sample it
    if (!f->Ks.isZero() && depth<4) {
        color += specularReflection (isect, f, depth+1);
    }

    color += directLighting(isect, f);

    return color;
};