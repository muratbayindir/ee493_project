#include "main.h"
#include <cJSON.h>

extern const char *device_id;
update_data_t client_data;

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
