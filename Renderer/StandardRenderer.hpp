//
// Created by jafmalheiro on 23/03/2023.
//

#ifndef VI_RT_STANDARDRENDERER_HPP
#define VI_RT_STANDARDRENDERER_HPP

#include "renderer.hpp"

class StandardRenderer: public Renderer {
public:
    StandardRenderer (Camera *cam, Scene * scene, Image * img, Shader *shd): Renderer(cam, scene, img, shd) {}
    void Render ();
};

#endif //VI_RT_STANDARDRENDERER_HPP
