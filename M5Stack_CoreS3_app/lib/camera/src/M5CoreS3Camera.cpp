#include "M5CoreS3Camera.hpp"

#include <M5Unified.h>
#include <esp_camera.h>

M5CoreS3Camera CoreS3Camera;

static camera_config_t camera_config = {
    .pin_pwdn = -1,
    .pin_reset = -1,
    .pin_xclk = 2,
    .pin_sscb_sda = 12,
    .pin_sscb_scl = 11,

    .pin_d7 = 47,
    .pin_d6 = 48,
    .pin_d5 = 16,
    .pin_d4 = 15,
    .pin_d3 = 42,
    .pin_d2 = 41,
    .pin_d1 = 40,
    .pin_d0 = 39,

    .pin_vsync = 46,
    .pin_href = 38,
    .pin_pclk = 45,

    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_RGB565,
    //.frame_size = FRAMESIZE_QVGA, // QVGA(320x240)
    .frame_size = FRAMESIZE_QQVGA,
    .jpeg_quality = 0,
    .fb_count = 2,
    .fb_location = CAMERA_FB_IN_PSRAM,
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
};

M5CoreS3Camera::M5CoreS3Camera()
{
}

esp_err_t M5CoreS3Camera::begin()
{
    // initialize the camera
    M5.In_I2C.release();
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK)
    {
        // Serial.println("Camera Init Failed");
        M5.Display.println("Camera Init Failed");
        return err;
    }

    return ESP_OK;
}

bool M5CoreS3Camera::get()
{
    fb = esp_camera_fb_get();
    if (fb == nullptr)
    {
        // Serial.println("Camera Capture Failed");
        // M5.Display.println("Camera Capture Failed");
        return false;
    }

    return true;
}

bool M5CoreS3Camera::free()
{
    if (fb != nullptr)
    {
        esp_camera_fb_return(fb);
        return true;
    }
    return false;
}
