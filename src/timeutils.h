#include <Arduino.h>

void setTime(time_t timestamp, const String &tz);
void setTimeZone(const String &tz);
bool syncNTPTime(const String &timezone);