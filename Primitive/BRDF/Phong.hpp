//
//  Phong.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 07/02/2023.
//

#ifndef PHONG_HPP
#define PHONG_HPP

#include "BRDF.hpp"
#include "RGB.hpp"
#include "vector.hpp"
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>

class Phong : public BRDF {
public:
    Phong() = default;

    RGB Kd;  // Diffuse reflectance
    RGB Ks;  // Specular reflectance
    float Ns{};  // Phong exponent
    RGB Ka, Kt;

    Phong(const RGB &Kd2, const RGB &Ks2, float _ns) : Kd(Kd2), Ks(Ks2), Ns(_ns) {}

    Vector normalize(const Vector& v) {
        float length = std::sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
        return {v.X / length, v.Y / length, v.Z / length};
    }

    RGB f (Vector wi, Vector wo, const BRDF_TYPES = BRDF_ALL) override {
        Vector h = normalize(wi + wo);
        float dotProduct = std::max(0.f, wi.dot(h));
        float spec = std::pow(dotProduct, Ns);
        return (Kd / M_PI) + (Ks * spec * (Ns + 2) / (2 * M_PI));
    }

    RGB Sample_f (Vector wi, float *prob, Vector *wo, const BRDF_TYPES = BRDF_ALL) override {
        // Generate a random direction over the hemisphere
        *wo = Vector::generateRandomHemisphereDirection();

        // Calculate the PDF of this direction
        *prob = 1 / (2 * M_PI); // For a uniform hemisphere distribution, the PDF is 1 / (2 * PI)

        return f(wi, *wo);
    }

};

#endif // PHONG_HPP
