#include "main.h"
#include <cJSON.h>

extern const char *device_id;

update_data_t client_data;
extern SemaphoreHandle_t xSem_client_data;
extern bool dataReadyToBeSend;
extern bool dataSent;

char *build_payload()
{
    char *serializedStr = NULL;
    cJSON *jsonRoot = NULL;
    cJSON *jsonDeviceId = NULL;
    cJSON *jsonTimeStamp = NULL;
    cJSON *jsonPayload = NULL;
    cJSON *jsonPayloadType = NULL;
    cJSON *jsonRssiInfos = NULL;
    cJSON *jsonRssiInfo = NULL;
    cJSON *jsonRssiValue = NULL;
    cJSON *jsonRssiTargetDevice = NULL;

    jsonRoot = cJSON_CreateObject();
    if (jsonRoot == NULL)
        goto END;

    jsonDeviceId = cJSON_CreateString(device_id);
    if (jsonDeviceId == NULL)
        goto END;

    /* after creation was successful, immediately add it to the root,
     * thereby transferring ownership of the pointer to it */
    cJSON_AddItemToObject(jsonRoot, "device_id", jsonDeviceId);

    jsonTimeStamp = cJSON_CreateNumber(xTaskGetTickCount() * portTICK_PERIOD_MS);
    if (jsonTimeStamp == NULL)
        goto END;
    cJSON_AddItemToObject(jsonRoot, "timestamp", jsonTimeStamp);

    jsonPayload = cJSON_CreateObject();
    if (jsonPayload == NULL)
        goto END;
    cJSON_AddItemToObject(jsonRoot, "payload", jsonPayload);

    jsonPayloadType = cJSON_CreateString("rssi_info");
    if (jsonPayloadType == NULL)
        goto END;
    cJSON_AddItemToObject(jsonPayload, "type", jsonPayloadType);

    jsonRssiInfos = cJSON_CreateArray();
    if (jsonRssiInfos == NULL)
        goto END;
    cJSON_AddItemToObject(jsonPayload, "rssi_infos", jsonRssiInfos);

    for (int i = 0; i < client_data.rssiInfosLen; i++)
    {
        jsonRssiInfo = cJSON_CreateObject();
        if (jsonRssiInfo == NULL)
            goto END;
        cJSON_AddItemToArray(jsonRssiInfos, jsonRssiInfo);

        jsonRssiValue = cJSON_CreateNumber(client_data.rssiInfos[i].value);
        if (jsonRssiValue == NULL)
            goto END;
        cJSON_AddItemToObject(jsonRssiInfo, "value", jsonRssiValue);

        jsonRssiTargetDevice = cJSON_CreateString(client_data.rssiInfos[i].targetName);
        if (jsonRssiTargetDevice == NULL)
            goto END;
        cJSON_AddItemToObject(jsonRssiInfo, "target_name", jsonRssiTargetDevice);
    }

    serializedStr = cJSON_Print(jsonRoot);
    if (serializedStr == NULL)
        ESP_LOGE(DEVICE_TAG, "Failed to print json : errno %d", errno);

END:

    cJSON_Delete(jsonRoot);

    return serializedStr;
}

void update_task(void)
{
    char *payload = NULL;
    int payload_len;
    int addr_family = 0;
    int ip_protocol = 0;

    while (1)
    {
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
            break;
        }
        ESP_LOGI(DEVICE_TAG, "Socket created, sending to %s:%d", SERVER_ADDRESS, SERVER_UPDATER_PORT);

        wifi_ap_record_t wifidata;

        while (1)
        {
            payload = NULL;

            while (dataReadyToBeSend == false)
                vTaskDelay(25 / portTICK_PERIOD_MS);

            if (xSem_client_data != NULL)
            {
                if (xSemaphoreTake(xSem_client_data, (TickType_t)10) == pdTRUE)
                {
                    payload = build_payload();

                    xSemaphoreGive(xSem_client_data);
                }
            }

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
                    dataSent = true;
                    dataReadyToBeSend = false;
                }
                free(payload);
            }

            vTaskDelay(25 / portTICK_PERIOD_MS);
        }

        if (sock != -1)
        {
            ESP_LOGE(DEVICE_TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}