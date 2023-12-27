//
//  perspective.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef perspective_hpp
#define perspective_hpp

#include "camera.hpp"
#include "ray.hpp"
#include "vector.hpp"

class Perspective: public Camera {
    Point Eye, At;
    Vector Up;
    float fovW, fovH;
    int W, H;
    float dist;
    Vector forward, right, up;
    float c2w[3][3];  // camera 2 world transform
public:
    Perspective (Point Eye, Point At, Vector Up, int W, int H, float fovW, float fovH);
    bool GenerateRay(int x, int y, Ray *r, const float *cam_jitter = nullptr);
    void getResolution(int *outW, int *outH) {
        *outW = W;
        *outH = H;
    }
};

#endif /* perspective_hpp */
