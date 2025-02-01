#pragma once

#include <esp_camera.h>

class M5CoreS3Camera
{
public:
    camera_fb_t *fb;

    M5CoreS3Camera();
    esp_err_t begin();
    bool get();
    bool free();
};

extern M5CoreS3Camera CoreS3Camera;
