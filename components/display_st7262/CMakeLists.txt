idf_component_register(
    SRCS "display_st7262.c" "rgb565.c"
    INCLUDE_DIRS "include"
    REQUIRES driver esp_lcd lvgl
)
