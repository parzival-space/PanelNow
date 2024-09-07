#include "Arduino.h"
#include "config.h"
#include "ESP8266WiFi.h"
#include "espnow.h"

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

void onDataRecv(uint8_t *mac, uint8_t *data, uint8_t len)
{
    Serial.print("Data received: ");
    for (int i = 0; i < len; i++)
    {
        Serial.print((char)data[i]);
    }
    Serial.println();
}

void setup()
{
    Serial.begin(SERIAL_BAUD);
    Serial.println("Starting...");

    // start Wi-Fi and set mac
    WiFi.mode(WIFI_STA);
    const String oldMac = WiFi.macAddress();
    wifi_set_macaddr(STATION_IF, BRIDGE_MAC);
    Serial.println("Updated device MAC: " + oldMac + " -> " + WiFi.macAddress());

    // init ESP-NOW
    Serial.println("Initializing ESP-NOW...");
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // complete esp now setup
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_send_cb(onDataSent);

    // register peer
    esp_now_add_peer(PANEL_MAC, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

    // register callback for incoming data
    esp_now_register_recv_cb(onDataRecv);

}

void loop()
{
    delay(1000);
}
