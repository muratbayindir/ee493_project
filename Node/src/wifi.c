#include "main.h"

extern update_data_t client_data;
SemaphoreHandle_t xSem_client_data = NULL;

/* Initialize Wi-Fi as sta and set scan method */
void wifi_task(void)
{
    // ESP_ERROR_CHECK(esp_netif_init());
    // ESP_ERROR_CHECK(esp_event_loop_create_default());
    // esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    // assert(sta_netif);

    // wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    // ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // // Initialize and start WiFi
    // wifi_config_t wifi_config = {
    //     .sta = {
    //         .ssid = WIFI_SSID,
    //         .password = WIFI_PASS
    //     },
    // };
    // ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    // ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    // ESP_ERROR_CHECK(esp_wifi_start());
    // ESP_ERROR_CHECK(esp_wifi_connect());

    // xTaskCreate(&update_task, "update_task", 4096, NULL, 5, NULL);

    // while(true) {
    // 	wifi_ap_record_t wifidata;

    // 	if (esp_wifi_sta_get_ap_info(&wifidata) == 0){
    //         ESP_LOGI(DEVICE_TAG, "rssi=%d\r\n", wifidata.rssi);
    //         client_data.rssiInfos[0].value = wifidata.rssi;
    //         client_data.rssiInfos[0].targetName = WIFI_SSID;
    //         client_data.rssiInfosLen = 1;
    // 	}

    // 	vTaskDelay(100 / portTICK_PERIOD_MS);
    // }

    xSem_client_data = xSemaphoreCreateMutex();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    uint16_t number = MAX_RSSI_INFO_LEN;
    wifi_ap_record_t ap_info[MAX_RSSI_INFO_LEN];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));

    // Initialize and start WiFi
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .scan_method = WIFI_FAST_SCAN
            },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_set_protocol(ESP_IF_WIFI_STA, WIFI_PROTOCOL_LR));
    // ESP_ERROR_CHECK(esp_wifi_set_protocol(ESP_IF_WIFI_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N));
    ESP_ERROR_CHECK(esp_wifi_start());
    // ESP_ERROR_CHECK(esp_wifi_connect());

    xTaskCreate(&update_task, "update_task", 4096, NULL, 5, NULL);

    while (true)
    {
#if 1
        // ESP_ERROR_CHECK(esp_wifi_disconnect());
        // ESP_ERROR_CHECK(esp_wifi_set_protocol(ESP_IF_WIFI_STA, WIFI_PROTOCOL_LR));

        ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
        ESP_LOGI(DEVICE_TAG, "Total APs scanned = %u", ap_count);

        if (xSem_client_data != NULL)
        {
            if (xSemaphoreTake(xSem_client_data, (TickType_t)10) == pdTRUE)
            {
                int i;
                for (i = 0; (i < MAX_RSSI_INFO_LEN) && (i < ap_count); i++)
                {
                    if(strcmp((const char *) ap_info[i].ssid, WIFI_SSID) == 0)
                    {
                        ESP_LOGI(DEVICE_TAG, "SSID \t\t%s", ap_info[i].ssid);
                        ESP_LOGI(DEVICE_TAG, "RSSI \t\t%d", ap_info[i].rssi);
                        ESP_LOGI(DEVICE_TAG, "Channel \t\t%d\n", ap_info[i].primary);
                    }
                    client_data.rssiInfos[i].value = ap_info[i].rssi;
                    client_data.rssiInfos[i].targetName = (char *) ap_info[i].ssid;
                }
                client_data.rssiInfosLen = i;

                xSemaphoreGive(xSem_client_data);
            }
        }

        // ESP_ERROR_CHECK(esp_wifi_set_protocol(ESP_IF_WIFI_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N));
        // ESP_ERROR_CHECK(esp_wifi_connect());

        vTaskDelay(50 / portTICK_PERIOD_MS);
#else


        if (xSem_client_data != NULL && xSemaphoreTake(xSem_client_data, (TickType_t)10) == pdTRUE)
        {
            wifi_ap_record_t wifidata;

            if (esp_wifi_sta_get_ap_info(&wifidata) == 0)
            {
                ESP_LOGI(DEVICE_TAG, "rssi=%d\r\n", wifidata.rssi);
                client_data.rssiInfos[0].value = wifidata.rssi;
                client_data.rssiInfos[0].targetName = WIFI_SSID;
                client_data.rssiInfosLen = 1;
            }

            xSemaphoreGive(xSem_client_data);

            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
#endif
    }
}
