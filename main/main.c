#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "album_manager.h"
#include "board_hal.h"
#include "color_palette.h"
#include "config.h"
#include "config_manager.h"
#include "crash_log.h"
#include "debug_log.h"
#include "display_manager.h"
#include "display_st7262.h"
#include "driver/gpio.h"
#include "esp_app_desc.h"
#include "esp_heap_caps.h"
#include "esp_littlefs.h"
#include "esp_log.h"
#include "esp_sntp.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "esp_vfs_dev.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// External RTC support
#ifdef CONFIG_EXT_RTC_ENABLED
#include "ext_rtc.h"
#endif

#include "ha_integration.h"
#include "http_server.h"
#include "image_processor.h"
#include "mdns_service.h"
#include "memfs.h"
#include "nvs_flash.h"
#include "ota_manager.h"
#include "periodic_tasks.h"
#include "power_manager.h"
#include "processing_settings.h"
#include "splash_screen.h"
#include "storage.h"
#include "utils.h"
#include "wifi_manager.h"
#include "wifi_provisioning.h"

static const char *TAG = "main";

void lvgl_port_init(void);

// Log the current wall-clock (defined below; forward-declared so the SNTP
// callback above the definition can use it too).
static void log_wall_clock(const char *label);
