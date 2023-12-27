//
// Created by kiko on 13/05/2023.
//

#include "Render.h"

__global__
void renderKernel(RGB* pixels, Camera* cam, Scene* scene, Shader* shd, Image* img, int W, int H, int spp) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if(x >= W || y >= H) return;

    RGB color(0., 0., 0.);
    Ray primary;
    Intersection isect;
    bool intersected;
    int depth = 0;

    for (int ss = 0; ss < spp; ss++) {
        float jitter[2];
        jitter[0] = getRandom(0, 1);
        jitter[1] = getRandom(0, 1);

        // Generate Ray (camera)
        cam->GenerateRay(x, y, &primary, jitter);

        // trace ray (scene)
        intersected = scene->trace(primary, &isect);

        // shade this intersection (shader)
        RGB this_color = shd->shade(intersected, isect, depth);

        color += this_color;
    }
    color = color / spp;

    // write the result into the image frame buffer (image)
    img->set(x, y, color);
}

void Render::Render_() {
    int W = 0, H = 0;  // resolution

    // get resolution from the camera
    Perspective* perspCam = dynamic_cast<Perspective*>(cam);
    perspCam->getResolution(&W, &H);

    RGB* dev_pixels;
    cudaMalloc(&dev_pixels, W * H * sizeof(RGB));

    dim3 threadsPerBlock(16, 16);
    dim3 numBlocks((W + threadsPerBlock.x - 1) / threadsPerBlock.x,
                   (H + threadsPerBlock.y - 1) / threadsPerBlock.y);
    renderKernel<<<numBlocks, threadsPerBlock>>>(dev_pixels, cam, scene, shd, img, W, H, spp);

    cudaMemcpy(img->pixels, dev_pixels, W * H * sizeof(RGB), cudaMemcpyDeviceToHost);

    cudaFree(dev_pixels);
}
