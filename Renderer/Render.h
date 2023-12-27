//
// Created by kiko on 13/05/2023.
//

#ifndef VI_RT_RENDER_H
#define VI_RT_RENDER_H


#include "camera.hpp"
#include "scene.hpp"
#include "image.hpp"
#include "../Shader/Shader.hpp"
#include "renderer.hpp"

class Render: public Renderer {
    Render(Camera *cam, Scene * scene, Image * img, Shader *shd): Renderer(cam, scene, img, shd) {}
    void Render_ ();

    float getRandom(int min, int max) {
        srand(time(nullptr)); // Seed the random number generator with the current time

        float random = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

        return min + random * (max - min);
    };
};


#endif //VI_RT_RENDER_H
