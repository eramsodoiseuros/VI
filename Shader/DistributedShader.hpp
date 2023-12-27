//
// Created by jafmalheiro on 12/05/2023.
//

#ifndef VI_RT_DISTRIBUTEDSHADER_HPP
#define VI_RT_DISTRIBUTEDSHADER_HPP


#include "Shader.hpp"
#include "Phong.hpp"

class DistributedShader: public Shader {
    RGB background;
    RGB directLighting(Intersection isect, Phong *f);
    RGB specularReflection(Intersection isect, Phong *f);
public:
    DistributedShader(Scene *scene, RGB bg) : background(bg), Shader(scene) {}
    RGB shade(bool intersected, Intersection isect, int depth);
    RGB pointLight(Intersection isect, Phong *f, Light* l, RGB color);

    RGB areaLight(Intersection isect, Phong *f, Light *l, RGB color,
                  std::uniform_real_distribution<float> distribution,
                  std::default_random_engine generator);
};

#endif //VI_RT_DISTRIBUTEDSHADER_HPP
