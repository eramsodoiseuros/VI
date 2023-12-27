//
// Created by jafmalheiro on 23/03/2023.
//

#ifndef VI_RT_SHADER_HPP
#define VI_RT_SHADER_HPP

#include "scene.hpp"
#include "RGB.hpp"

class Shader {
protected:
    Scene *scene;
public:
    Shader (Scene *_scene): scene(_scene) {}
    ~Shader () {}
    virtual RGB shade (bool intersected, Intersection isect, int depth) {return RGB();}
};

#endif //VI_RT_SHADER_HPP
