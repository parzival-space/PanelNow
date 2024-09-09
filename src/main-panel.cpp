#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "espnow.h"

#include "config.h"
#include "communication.h"

void onDataSent(uint8_t *mac_addr, uint8_t status)
{
    Serial.print("Last Packet Send Status: ");
    if (status == 0)
    {
        Serial.println("Delivery success");
    }
    else
    {
        Serial.println("Delivery fail");
    }
}

char message[];
void onDataRecv(uint8_t *mac, uint8_t *data, uint8_t len)
{
    memcpy(&message, data, sizeof(message));

    Serial.print("Data received: ");
    Serial.print("message: ");
    Serial.print(frame.message);
    Serial.print(", number: ");
    Serial.print(frame.number);
    Serial.println();
}

void setup()
{
    Serial.begin(SERIAL_BAUD);
    Serial.println("Starting...");

    // Start Wi-Fi and set MAC address
    WiFi.mode(WIFI_STA);
    wifi_set_macaddr(STATION_IF, PANEL_MAC);
    Serial.println("Updated device MAC: " + WiFi.macAddress() + " -> " + PANEL_MAC[0] + ":" + PANEL_MAC[1] + ":" + PANEL_MAC[2] + ":" + PANEL_MAC[3] + ":" + PANEL_MAC[4] + ":" + PANEL_MAC[5]);

    // Initialize ESP-NOW
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Complete ESP-NOW setup
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_send_cb(onDataSent);
    esp_now_add_peer(BRIDGE_MAC, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    esp_now_register_recv_cb(onDataRecv);
}

void loop()
{
    delay(1000);
}
