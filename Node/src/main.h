#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

/* Constants that aren't configurable in menuconfig */
#define SERVER_ADDRESS "192.168.1.3"
#define SERVER_UPDATER_PORT 2828

#define DEVICE_TAG "example_001"

#define WIFI_SSID "SUPERONLINE_WiFi_4766"
#define WIFI_PASS "zbQ4eZJZAu4j"

#define MAX_RSSI_INFO_LEN   30

typedef struct rssi_info
{
    int value;
    const char* targetName;
}rssi_info_t;

typedef struct update_data
{
    rssi_info_t rssiInfos[MAX_RSSI_INFO_LEN];
    int rssiInfosLen;
}update_data_t;

void update_task(void);
void wifi_task(void);


