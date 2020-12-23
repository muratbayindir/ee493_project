#include "main.h"

const char *device_id = "DEVICE_001";

// #define ENABLE_LONG_RANGE
// #define ENABLE_PASSWORD

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_init_config_t wifiInitializationConfig = WIFI_INIT_CONFIG_DEFAULT();

    esp_wifi_init(&wifiInitializationConfig);

    esp_wifi_set_storage(WIFI_STORAGE_RAM);

    esp_wifi_set_mode(WIFI_MODE_AP);

    wifi_config_t ap_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .channel = 0,
#ifdef ENABLE_PASSWORD
            .authmode = WIFI_AUTH_WPA2_PSK,
            .password = WIFI_PASS,
#endif
            .ssid_hidden = 0,
            .max_connection = 3,
            .beacon_interval = 100}};

    esp_wifi_set_config(ESP_IF_WIFI_AP, &ap_config);

#ifdef ENABLE_LONG_RANGE
    ESP_ERROR_CHECK(esp_wifi_set_protocol(ESP_IF_WIFI_AP, WIFI_PROTOCOL_LR));
#endif

    esp_wifi_start();

    while(true) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
