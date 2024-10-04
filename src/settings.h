#include <Arduino.h>
#include <time.h>
// Settings structure
struct Settings
{
    String wifiSSID;
    String wifiPassword;
    bool useNTP;
    String timezone;
    bool alarmEnabled;
    time_t alarmTime;
    bool weekdays[7];
};

// Proto
void loadSettings(Settings &settings);
void saveSettings(Settings settings);