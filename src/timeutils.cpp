#include <Arduino.h>
#include <timeutils.h>

// NTP Server
const char *ntpServer = "pool.ntp.org";

void setTime(time_t timestamp, const String &tz)
{
    struct timeval tv;
    tv.tv_sec = timestamp;
    tv.tv_usec = 0;
    settimeofday(&tv, NULL);
    Serial.println("Time set manually: " + String(ctime(&timestamp)));

    // Update the RTC
    struct tm timeinfo;
    localtime_r(&timestamp, &timeinfo);
    setTimeZone(tz);
}

void setTimeZone(const String &tz)
{
    setenv("TZ", tz.c_str(), 1);
    tzset();
    Serial.println("Timezone set to: " + tz);
}

bool syncNTPTime(const String &timezone)
{
    configTime(0, 0, ntpServer); // First, sync without timezone
    setTimeZone(timezone);

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return false;
    }

    time_t now;
    time(&now);
    Serial.println("Time synchronized: " + String(ctime(&now)));
    return true;
}