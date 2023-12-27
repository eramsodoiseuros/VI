//
// Created by jafmalheiro on 14/05/2023.
//

#ifndef VI_RT_PATHTRACERSHADER_HPP
#define VI_RT_PATHTRACERSHADER_HPP


#include "Shader.hpp"
#include "Phong.hpp"

class PathTracerShader: public Shader {
    RGB background;
    RGB directLighting(Intersection isect, Phong *f, std::uniform_real_distribution<float> distribution, std::default_random_engine generator);
    RGB specularReflection(Intersection isect, Phong *f, int depth, std::uniform_real_distribution<float> distribution, std::default_random_engine generator);
    RGB diffuseReflection(Intersection isect, Phong *f, int depth, std::uniform_real_distribution<float> distribution, std::default_random_engine generator);
    float continue_p;
    int MAX_DEPTH;
public:
    PathTracerShader (Scene *scene, RGB bg): background(bg), Shader(scene) {continue_p = 0.5f; MAX_DEPTH=2;}

    RGB shade(bool intersected, Intersection isect, int depth) override;
    RGB pointLight(Intersection isect, Phong *f, Light* l, RGB color);

    RGB areaLight(Intersection isect, Phong *f, Light *l, RGB color, std::uniform_real_distribution<float> distribution,
                  std::default_random_engine generator);
};



#endif //VI_RT_PATHTRACERSHADER_HPP
