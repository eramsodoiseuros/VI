//
// Created by jafmalheiro on 23/03/2023.
//

#ifndef VI_RT_AMBIENTSHADER_HPP
#define VI_RT_AMBIENTSHADER_HPP

#include "shader.hpp"

class AmbientShader: public Shader {
    RGB background;
public:
    AmbientShader (Scene *scene, RGB bg): background(bg), Shader(scene) {}
    RGB shade (bool intersected, Intersection isect, int depth);
};

#endif //VI_RT_AMBIENTSHADER_HPP
