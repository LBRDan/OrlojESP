#include <wifiutils.h>

void createAccessPoint()
{
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    Serial.println("Access Point Created");
    Serial.print("SSID: ");
    Serial.println(AP_SSID);
    Serial.print("Password: ");
    Serial.println(AP_PASSWORD);
}