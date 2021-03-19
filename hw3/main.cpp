/* WiFi Example
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SocketAddress.h"
#include "TCPSocket.h"
#include "mbed.h"

// Sensors drivers present in the BSP library
#include "stm32l475e_iot01_accelero.h"
#include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_hsensor.h"
#include "stm32l475e_iot01_magneto.h"
#include "stm32l475e_iot01_psensor.h"
#include "stm32l475e_iot01_tsensor.h"

#define WIFI_IDW0XX1 2

#if (defined(TARGET_DISCO_L475VG_IOT01A) || defined(TARGET_DISCO_F413ZH))
#include "ISM43362Interface.h"
ISM43362Interface wifi(false);

#else // External WiFi modules

#if MBED_CONF_APP_WIFI_SHIELD == WIFI_IDW0XX1
#include "SpwfSAInterface.h"
SpwfSAInterface wifi(MBED_CONF_APP_WIFI_TX, MBED_CONF_APP_WIFI_RX);
#endif // MBED_CONF_APP_WIFI_SHIELD == WIFI_IDW0XX1

#endif

static BufferedSerial serial_port(USBTX, USBRX);
FileHandle *mbed::mbed_override_console(int fd) { return &serial_port; }

const char *sec2str(nsapi_security_t sec) {
    switch (sec) {
        case NSAPI_SECURITY_NONE:
            return "None";
        case NSAPI_SECURITY_WEP:
            return "WEP";
        case NSAPI_SECURITY_WPA:
            return "WPA";
        case NSAPI_SECURITY_WPA2:
            return "WPA2";
        case NSAPI_SECURITY_WPA_WPA2:
            return "WPA/WPA2";
        case NSAPI_SECURITY_UNKNOWN:
        default:
            return "Unknown";
    }
}

int scan_demo(WiFiInterface *wifi) {
    WiFiAccessPoint *ap;

    printf("Scan:\n");

    int count = wifi->scan(NULL, 0);
    printf("%d networks available.\n", count);

    /* Limit number of network arbitrary to 15 */
    count = count < 15 ? count : 15;

    ap = new WiFiAccessPoint[count];
    count = wifi->scan(ap, count);
    for (int i = 0; i < count; i++) {
        printf("Network: %s secured: %s BSSID: %hhX:%hhX:%hhX:%hhx:%hhx:%hhx "
               "RSSI: %hhd Ch: %hhd\n",
               ap[i].get_ssid(), sec2str(ap[i].get_security()),
               ap[i].get_bssid()[0], ap[i].get_bssid()[1], ap[i].get_bssid()[2],
               ap[i].get_bssid()[3], ap[i].get_bssid()[4], ap[i].get_bssid()[5],
               ap[i].get_rssi(), ap[i].get_channel());
    }

    delete[] ap;
    return count;
}

#define SCALE_MULTIPLIER 2

void sensor_reading(NetworkInterface *net) {
    TCPSocket socket;
    nsapi_error_t response;

    int sample_num = 0;
    int16_t pDataXYZ[3] = {0};
    char acc_json[1024];

    SocketAddress a;
    net->get_ip_address(&a);
    printf("IP address: %s\n",
           a.get_ip_address() ? a.get_ip_address() : "None");
    socket.open(net);
    net->gethostbyname("linux1.csie.ntu.edu.tw", &a);
    a.set_port(54321);
    response = socket.connect(a);

    while (1) {
        ++sample_num;
        BSP_ACCELERO_AccGetXYZ(pDataXYZ);
        float x = pDataXYZ[0] * SCALE_MULTIPLIER;
        float y = pDataXYZ[1] * SCALE_MULTIPLIER;
        float z = pDataXYZ[2] * SCALE_MULTIPLIER;
        int len = sprintf(acc_json, "{\"x\":%f,\"y\":%f,\"z\":%f,\"s\":%d}",
                          (float)((int)(x * 10000) / 10000),
                          (float)((int)(y * 10000) / 10000),
                          (float)((int)(z * 10000) / 10000), sample_num);
        response = socket.send(acc_json, len);
        if (response < 0) {
            printf("Error sending: %d\n", response);
        }
        ThisThread::sleep_for(1s);
    }
}

int main() {
    int count = 0;

    printf("WiFi example\n\n");

    count = scan_demo(&wifi);
    if (count == 0) {
        printf("No WIFI APNs found - can't continue further.\n");
        return -1;
    }

    printf("\nConnecting to %s...\n", MBED_CONF_APP_WIFI_SSID);
    int ret = wifi.connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD,
                           NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
        printf("\nConnection error\n");
        return -1;
    }

    printf("Success\n\n");
    printf("MAC: %s\n", wifi.get_mac_address());
    printf("IP: %s\n", wifi.get_ip_address());
    printf("Netmask: %s\n", wifi.get_netmask());
    printf("Gateway: %s\n", wifi.get_gateway());
    printf("RSSI: %d\n\n", wifi.get_rssi());

    sensor_reading(&wifi);

    wifi.disconnect();

    printf("\nDone\n");
}
