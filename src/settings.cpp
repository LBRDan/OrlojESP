#include <Arduino.h>
#include <settings.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

void loadSettings(Settings &settings)
{
    File file = SPIFFS.open("/settings.json", "r");
    if (!file)
    {
        // If file doesn't exist, use default settings
        settings.wifiSSID = "";
        settings.wifiPassword = "";
        settings.useNTP = true;
        settings.timezone = "UTC";
        settings.alarmEnabled = false;
        settings.alarmTime = 0;
        for (int i = 0; i < 7; i++)
        {
            settings.weekdays[i] = false;
        }
        return;
    }

    JsonDocument doc;
    Serial.println("Reading settings file...");
    Serial.println(file.size());
    Serial.println(file.name());
    Serial.println(file.path());
    Serial.println(file.readBytesUntil('\n', (char *)0, 0));
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error)
    {
        Serial.println("Failed to read settings file");
        settings.wifiSSID = "";
        settings.wifiPassword = "";
        settings.useNTP = true;
        settings.timezone = "UTC";
        settings.alarmEnabled = false;
        settings.alarmTime = 0;
        for (int i = 0; i < 7; i++)
        {
            settings.weekdays[i] = false;
        }
        saveSettings(settings);
        return;
    }

    settings.wifiSSID = doc["wifiSSID"].as<String>();
    settings.wifiPassword = doc["wifiPassword"].as<String>();
    settings.useNTP = doc["useNTP"];
    settings.timezone = doc["timezone"].as<String>();
    settings.alarmEnabled = doc["alarmEnabled"];
    settings.alarmTime = doc["alarmTime"];
    for (int i = 0; i < 7; i++)
    {
        settings.weekdays[i] = doc["weekdays"][i];
    }
}

void saveSettings(Settings settings)
{
    JsonDocument doc;

    doc["wifiSSID"] = settings.wifiSSID;
    doc["wifiPassword"] = settings.wifiPassword;
    doc["useNTP"] = settings.useNTP;
    doc["timezone"] = settings.timezone;
    doc["alarmEnabled"] = settings.alarmEnabled;
    doc["alarmTime"] = settings.alarmTime;
    doc["weekdays"].to<JsonArray>();
    for (int i = 0; i < 7; i++)
    {
        doc["weekdays"].add(settings.weekdays[i]);
    }

    File file = SPIFFS.open("/settings.json", "w");
    if (!file)
    {
        Serial.println("Failed to open settings file for writing");
        return;
    }

    if (serializeJson(doc, file) == 0)
    {
        Serial.println("Failed to write settings to file");
    }
    file.close();
}
