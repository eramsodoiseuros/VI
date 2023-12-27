//
// Created by jafmalheiro on 23/03/2023.
//

#include "StandardRenderer.hpp"
#include "perspective.hpp"
#include <omp.h>

const float spp = 256.0f;

void StandardRenderer::Render() {
    int W = 0, H = 0;  // resolution

    // get resolution from the camera
    Perspective* perspCam = dynamic_cast<Perspective*>(cam);
    perspCam->getResolution(&W, &H);

    int maxThreads = omp_get_max_threads();
    std::cout << "Max available OpenMP threads: " << maxThreads << std::endl;

    int targetThreads = static_cast<int>(maxThreads * 0.8);  // we don't want to use 100% of the power
    if (targetThreads > 0) {
        targetThreads = 10;
        std::cout << "Will run with [" << targetThreads << "] OpenMP threads "<< std::endl;
        omp_set_num_threads(targetThreads);
    }

    // main rendering loop: get primary rays from the camera until done
#pragma omp parallel default(none) shared(perspCam, W, H)
    {
#pragma omp for schedule(guided, 5) // Parallelize outer loop
        for (int y = 0; y < H; y++) {  // loop over rows
            for (int x = 0; x < W; x++) { // loop over columns
                RGB color(0., 0., 0.);
                for (int ss = 0; ss < spp; ss++) {

                    float jitter[2];

                    jitter[0] = Vector::generateRandomFloat();
                    jitter[1] = Vector::generateRandomFloat();

                    // Generate Ray (camera)
                    Ray primary;
                    perspCam->GenerateRay(x, y, &primary, jitter);

                    // trace ray (scene)
                    Intersection isect;
                    bool intersected = scene->trace(primary, &isect);

                    // shade this intersection (shader)
                    RGB this_color = shd->shade(intersected, isect, 0);

                    color += this_color;
                }
                color = color / spp;

                // write the result into the image frame buffer (image)
                img->set(x, y, color);

            } // loop over columns
        }   // loop over rows
    }
}



