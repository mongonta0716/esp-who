set(COMPONENT_SRCS "app_main.cpp" "app_wifi.c" "app_camera.c" "app_httpd.c" "app_mdns.c" "app_led.cpp" "timer_cam_config.c" "battery.c")
set(COMPONENT_ADD_INCLUDEDIRS "include")

set(COMPONENT_REQUIRES
    esp32-camera
    esp-face
    nvs_flash
    esp_http_server
    fb_gfx
    mdns
    FastLED-idf
    esp_adc_cal
    )


set(COMPONENT_EMBED_FILES
        "www/index_ov2640.html.gz"
        "www/index_ov3660.html.gz"
        "www/index_ov5640.html.gz"
        "www/monitor.html.gz")

register_component()
