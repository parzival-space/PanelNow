#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "espnow.h"

#include "config.h"
#include "communication.h"

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);

void setup()
{
    Serial.begin(SERIAL_BAUD);

    // Start Wi-Fi and set MAC address
    WiFi.mode(WIFI_STA);
    wifi_set_macaddr(STATION_IF, PANEL_MAC);
    Serial.println("Updated device MAC: " + WiFi.macAddress() + " -> " + BRIDGE_MAC[0] + ":" + BRIDGE_MAC[1] + ":" + BRIDGE_MAC[2] + ":" + BRIDGE_MAC[3] + ":" + BRIDGE_MAC[4] + ":" + BRIDGE_MAC[5]);

    // Initialize ESP-NOW
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // set role of this device
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

    // set callback
    esp_now_register_send_cb(OnDataSent);

    // register peer
    esp_now_add_peer(PANEL_MAC, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

char message[] = "Hello World, from bridge!";
void loop()
{
    esp_now_send(PANEL_MAC, reinterpret_cast<uint8_t*>(&message), sizeof(message));
    delay(2000);
}

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
    Serial.print("\r\nDelivery Status: ");
    Serial.println(sendStatus == 0 ? "Delivered Successfully" : "Delivery Fail");
}