#include "main.h"
#include <cJSON.h>

extern const char *device_id;

update_data_t client_data;

char* build_payload()
{
    char *serializedStr;
    cJSON *jsonRoot = NULL;
    cJSON *jsonDeviceId = NULL;
    cJSON *jsonTimeStamp = NULL;
    cJSON *jsonPayload = NULL;
    cJSON *jsonPayloadType = NULL;
    cJSON *jsonRssiInfo = NULL;
    cJSON *jsonRssiValue = NULL;
    cJSON *jsonRssiTargetDevice = NULL;

    jsonRoot = cJSON_CreateObject();
    if(jsonRoot == NULL)
        return NULL;

    jsonDeviceId = cJSON_CreateString(device_id);
    if (jsonDeviceId == NULL)
        return NULL;

    /* after creation was successful, immediately add it to the root,
     * thereby transferring ownership of the pointer to it */
    cJSON_AddItemToObject(jsonRoot, "device_id", jsonDeviceId);

    jsonTimeStamp = cJSON_CreateNumber(xTaskGetTickCount() * portTICK_PERIOD_MS);
    if (jsonTimeStamp == NULL)
        return NULL;
    cJSON_AddItemToObject(jsonRoot, "timestamp", jsonTimeStamp);

    jsonPayload = cJSON_CreateObject();
    if(jsonPayload == NULL)
        return NULL;
    cJSON_AddItemToObject(jsonRoot, "payload", jsonPayload);

    jsonPayloadType = cJSON_CreateString("rssi_info");
    if(jsonPayloadType == NULL)
        return NULL;
    cJSON_AddItemToObject(jsonPayload, "type", jsonPayloadType);

    jsonRssiInfo = cJSON_CreateObject();
    if(jsonRssiInfo == NULL)
        return NULL;
    cJSON_AddItemToObject(jsonPayload, "rssi_info", jsonRssiInfo);

    jsonRssiValue = cJSON_CreateNumber(client_data.rssi);
    if(jsonRssiValue == NULL)
        return NULL;
    cJSON_AddItemToObject(jsonRssiInfo, "rssi_value", jsonRssiValue);

    jsonRssiTargetDevice = cJSON_CreateString("modem");
    if(jsonRssiTargetDevice == NULL)
        return NULL;
    cJSON_AddItemToObject(jsonRssiInfo, "target_device", jsonRssiTargetDevice);


    serializedStr = cJSON_Print(jsonRoot);
    if (serializedStr == NULL)
        ESP_LOGE(DEVICE_TAG, "Failed to print json : errno %d", errno);

    cJSON_Delete(jsonRoot);

    return serializedStr;
}

void update_task(void) 
{
    // char payload[128];
    char *payload;
    int payload_len;
    int addr_family = 0;
    int ip_protocol = 0;

    while (1) {

        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(SERVER_UPDATER_PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;

        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(DEVICE_TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(DEVICE_TAG, "Socket created, sending to %s:%d", SERVER_ADDRESS, SERVER_UPDATER_PORT);

        while (1) {
            
            if((payload = build_payload()) != NULL)
            {
                payload_len = strlen(payload);
                // payload_len = sprintf(payload, "{\"device_id\":\"%s\",\"rssi\":%d}", device_id, client_data.rssi);
                int err = sendto(sock, payload, payload_len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
                if (err < 0) {
                    ESP_LOGE(DEVICE_TAG, "Error occurred during sending: errno %d", errno);
                    break;
                }
                ESP_LOGI(DEVICE_TAG, "Message sent");
            }

            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }

        if (sock != -1) {
            ESP_LOGE(DEVICE_TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}