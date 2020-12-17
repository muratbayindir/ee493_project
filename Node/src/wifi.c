#include "main.h"

extern update_data_t client_data;

/* Initialize Wi-Fi as sta and set scan method */
void wifi_task(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Initialize and start WiFi
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
	ESP_ERROR_CHECK(esp_wifi_connect());

    xTaskCreate(&update_task, "update_task", 4096, NULL, 5, NULL);

	while(true) {
		wifi_ap_record_t wifidata;

		if (esp_wifi_sta_get_ap_info(&wifidata) == 0){
            ESP_LOGI(DEVICE_TAG, "rssi=%d\r\n", wifidata.rssi);
            client_data.rssi = wifidata.rssi;
		}

		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}
