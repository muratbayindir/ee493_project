#include "main.h"

#define ENABLE_SCAN
#define ENABLE_CONNECT
#define ENABLE_LONG_RANGE
// #define SHOW_SINGLE_AP

extern update_data_t client_data;
uint16_t number = MAX_RSSI_INFO_LEN;
wifi_ap_record_t ap_info[MAX_RSSI_INFO_LEN];

/* Initialize Wi-Fi as sta and set scan method */
void wifi_task(void)
{
    char *payload = NULL;
    int payload_len;
    int addr_family = 0;
    int ip_protocol = 0;

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));

    // Initialize and start WiFi
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .scan_method = WIFI_FAST_SCAN},
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
#ifdef ENABLE_LONG_RANGE
        ESP_ERROR_CHECK(esp_wifi_set_protocol(ESP_IF_WIFI_STA, WIFI_PROTOCOL_LR));
#endif
    ESP_ERROR_CHECK(esp_wifi_start());
#ifdef ENABLE_CONNECT
    ESP_ERROR_CHECK(esp_wifi_set_protocol(ESP_IF_WIFI_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N));
    ESP_ERROR_CHECK(esp_wifi_connect());
#endif /* ENABLE_CONNECT */

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(SERVER_UPDATER_PORT);
    addr_family = AF_INET;
    ip_protocol = IPPROTO_IP;

    int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
    if (sock < 0)
    {
        ESP_LOGE(DEVICE_TAG, "Unable to create socket: errno %d", errno);
        return;
    }
    ESP_LOGI(DEVICE_TAG, "Socket created, sending to %s:%d", SERVER_ADDRESS, SERVER_UPDATER_PORT);

    while (true)
    {
#ifdef ENABLE_SCAN
#ifdef ENABLE_CONNECT
        ESP_ERROR_CHECK(esp_wifi_disconnect());
#endif /* ENABLE_CONNECT */
#ifdef ENABLE_LONG_RANGE
#ifdef ENABLE_CONNECT
        ESP_ERROR_CHECK(esp_wifi_set_protocol(ESP_IF_WIFI_STA, WIFI_PROTOCOL_LR));
#endif /* ENABLE_CONNECT */
#endif
        ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
        ESP_LOGI(DEVICE_TAG, "Total APs scanned = %u", ap_count);
#endif

#ifdef ENABLE_SCAN
        int i;
        for (i = 0; (i < MAX_RSSI_INFO_LEN) && (i < ap_count); i++)
        {
#ifdef SHOW_SINGLE_AP
            if (strcmp((const char *)ap_info[i].ssid, WIFI_SSID) == 0)
            {
#endif
                ESP_LOGI(DEVICE_TAG, "SSID \t\t%s", ap_info[i].ssid);
                ESP_LOGI(DEVICE_TAG, "RSSI \t\t%d", ap_info[i].rssi);
                ESP_LOGI(DEVICE_TAG, "Channel \t\t%d\n", ap_info[i].primary);
#ifdef SHOW_SINGLE_AP
            }
#endif
            client_data.rssiInfos[i].value = ap_info[i].rssi;
            client_data.rssiInfos[i].targetName = (char *)ap_info[i].ssid;
        }
        client_data.rssiInfosLen = i;
#else
        wifi_ap_record_t wifidata;

        if (esp_wifi_sta_get_ap_info(&wifidata) == 0)
        {
            ESP_LOGI(DEVICE_TAG, "rssi=%d\r\n", wifidata.rssi);
            client_data.rssiInfos[0].value = wifidata.rssi;
            client_data.rssiInfos[0].targetName = WIFI_SSID;
            client_data.rssiInfosLen = 1;
        }
#endif

#ifdef ENABLE_SCAN
#ifdef ENABLE_LONG_RANGE
#ifdef ENABLE_CONNECT
        ESP_ERROR_CHECK(esp_wifi_set_protocol(ESP_IF_WIFI_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N));
#endif /* ENABLE_CONNECT */
#endif
#ifdef ENABLE_CONNECT
        ESP_ERROR_CHECK(esp_wifi_connect());
#endif /* ENABLE_CONNECT */
#endif

#ifdef ENABLE_CONNECT

        payload = build_payload();
        while (1)
        {
            if (payload != NULL)
            {
                payload_len = strlen(payload);
                int err = sendto(sock, payload, payload_len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
                if (err < 0)
                {
                    ESP_LOGE(DEVICE_TAG, "Error occurred during sending: errno %d", errno);
                }
                else
                {
                    ESP_LOGI(DEVICE_TAG, "Message sent");
                    free(payload);
                    break;
                }
            }

            vTaskDelay(25 / portTICK_PERIOD_MS);
        }
#endif /* ENABLE_CONNECT */

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    if (sock != -1)
    {
        ESP_LOGE(DEVICE_TAG, "Shutting down socket and restarting...");
        shutdown(sock, 0);
        close(sock);
    }
}
