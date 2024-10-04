#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

#include <time.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <settings.h>
#include <timeutils.h>
#include <display.h>
#include <wifiutils.h>

// ESP32 SS=5,SCL(SCK)=18,SDA(MOSI)=23,BUSY=15,RST=2,DC=0
// 2.9'' EPD Module
GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> display(GxEPD2_290_BS(/*CS=5*/ 5, /*DC=*/0, /*RST=*/2, /*BUSY=*/15)); // DEPG0290BS 128x296, SSD1680

// WiFi credentials
// const char *ssid = "Vodafone-C01330180";
// const char *password = "N6AEnxhJgf7csYP4";

// Global variable to track if a restart is pending
bool restartPending = false;
unsigned long restartTime = 0;

// Button pins
const int BUTTON_UP = 12;
const int BUTTON_DOWN = 14;
const int BUTTON_CONFIRM = 27;
const int BUTTON_BACK = 26;
const int BUTTON_ALARM_OFF = 25;

// Builtin led pin
const int LED_BUILTIN = 2;

// Settings
Settings settings;

// Menu states
enum MenuState
{
  SPLASH,
  SHOW_TIME,
  MAIN_MENU,
  SET_TIME,
  SET_ALARM
};

MenuState currentState = SHOW_TIME;
MenuState previousState = SPLASH;

AsyncWebServer server(80);

// Function prototypes
void displaySplashScreen();
void setupWiFi();

void handleButtons();
void updateDisplay();

void displayMainMenu();
void displaySetTime();
void displaySetAlarm();
void setupWebServer();
void handleSaveSettings(AsyncWebServerRequest *request);
void handleGetSettings(AsyncWebServerRequest *request);

void checkRestartPending();
void scheduleRestart();

void setup()
{
  Serial.begin(115200);

  // Turn off onboard LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Load settings
  loadSettings(settings);

  // Initialize display
  // display.init(115200, true, 2, false);
  display.init(115200, true, 50, false);
  display.setRotation(1);

  if (display.epd2.hasFastPartialUpdate)
  {
    Serial.println("Fast partial update supported");
  }
  else if (display.epd2.hasPartialUpdate)
  {
    Serial.println("Partial update supported");
  }
  else
  {
    Serial.println("Partial update not supported");
  }

  // Show splash screen
  displaySplashScreen();

  // Setup WiFi
  setupWiFi();

  // Setup web server
  setupWebServer();

  // Set timezone and sync time
  setTimeZone(settings.timezone);

  if (settings.useNTP)
  {
    if (!syncNTPTime(settings.timezone))
    {
      // If NTP sync fails, set time manually to 1 Jan 2021 00:00:00
      setTime(1609459200, settings.timezone);
    }
  }

  // Initialize buttons
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_CONFIRM, INPUT_PULLUP);
  pinMode(BUTTON_BACK, INPUT_PULLUP);
  pinMode(BUTTON_ALARM_OFF, INPUT_PULLUP);

  // Set font for regular display
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);

  // Initial full refresh
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
  } while (display.nextPage());
}

void loop()
{
  handleButtons();
  updateDisplay();
  checkRestartPending();
  delay(100); // Small delay to prevent excessive updates
}

void scheduleRestart()
{
  restartPending = true;
  restartTime = millis() + 5000; // Restart in 5 seconds
}

void checkRestartPending()
{
  if (restartPending && millis() >= restartTime)
  {
    Serial.println("Restarting...");
    ESP.restart();
  }
}

void displaySplashScreen()
{
  display.setFullWindow();
  display.setFont(&FreeMonoBold24pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    int16_t tbx, tby;
    uint16_t tbw, tbh;
    display.getTextBounds("OrlojESP", 0, 0, &tbx, &tby, &tbw, &tbh);
    uint16_t x = (display.width() - tbw) / 2;
    uint16_t y = (display.height() + tbh) / 2;
    display.setCursor(x, y);
    display.print("OrlojESP");
  } while (display.nextPage());
}

void setupWiFi()
{
  if (settings.wifiSSID.length() > 0)
  {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(settings.wifiSSID.c_str(), settings.wifiPassword.c_str());
    // WiFi.begin(ssid, password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20)
    {
      delay(500);
      Serial.print(".");
      attempts++;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("\nConnected to WiFi");
      return;
    }
  }

  // If we reach here, either no credentials were set or connection failed
  createAccessPoint();
  displayAccessPointInfo(display, AP_SSID, AP_PASSWORD);
}

void handleButtons()
{
  if (digitalRead(BUTTON_UP) == LOW)
  {
    // Handle UP button
  }
  if (digitalRead(BUTTON_DOWN) == LOW)
  {
    // Handle DOWN button
  }
  if (digitalRead(BUTTON_CONFIRM) == LOW)
  {
    // Handle CONFIRM button
  }
  if (digitalRead(BUTTON_BACK) == LOW)
  {
    // Handle BACK button
  }
  if (digitalRead(BUTTON_ALARM_OFF) == LOW)
  {
    // Handle ALARM OFF button
  }
}

void updateDisplay()
{
  if (currentState != previousState)
  {
    // Full refresh when changing states
    display.setFullWindow();
    display.firstPage();
    do
    {
      display.fillScreen(GxEPD_WHITE);
      switch (currentState)
      {
      case SHOW_TIME:
        showTimeAndDate(display, true);
        break;
      case MAIN_MENU:
        displayMainMenu();
        break;
      case SET_TIME:
        displaySetTime();
        break;
      case SET_ALARM:
        displaySetAlarm();
        break;
      }
    } while (display.nextPage());
    previousState = currentState;
  }
  else if (currentState == SHOW_TIME)
  {
    // Partial update for time/date display
    showTimeAndDate(display, false);
  }
}

void displayMainMenu()
{
  // Implement main menu display
}

void displaySetTime()
{
  // Implement set time interface
}

void displaySetAlarm()
{
  // Implement set alarm interface
}

// Add more helper functions as needed
void setupWebServer()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html"); });

  server.on("/save-settings", HTTP_POST, handleSaveSettings);
  server.on("/get-settings", HTTP_GET, handleGetSettings);

  server.begin();
}

void handleSaveSettings(AsyncWebServerRequest *request)
{
  bool needsRestart = false;
  if (request->hasParam("wifi-ssid", true) && request->hasParam("wifi-password", true))
  {
    settings.wifiSSID = request->getParam("wifi-ssid", true)->value();
    settings.wifiPassword = request->getParam("wifi-password", true)->value();
    needsRestart = true;
  }

  settings.useNTP = request->hasParam("use-ntp", true);
  settings.timezone = request->getParam("timezone", true)->value();

  if (!settings.useNTP)
  {
    String manualDatetime = request->getParam("manual-datetime", true)->value();
    struct tm tm = {0};
    strptime(manualDatetime.c_str(), "%Y-%m-%dT%H:%M", &tm);
    time_t timestamp = mktime(&tm);
    setTime(timestamp, settings.timezone);
  }
  else
  {
    setTimeZone(settings.timezone);
    syncNTPTime(settings.timezone);
  }

  settings.alarmEnabled = request->hasParam("enable-alarm", true);
  if (settings.alarmEnabled)
  {
    String alarmTime = request->getParam("alarm-time", true)->value();
    struct tm tm = {0};
    strptime(alarmTime.c_str(), "%H:%M", &tm);
    settings.alarmTime = mktime(&tm);

    for (int i = 0; i < 7; i++)
    {
      settings.weekdays[i] = request->hasParam("weekdays[]", true) &&
                             request->getParam("weekdays[]", true)->value().indexOf(String(i)) != -1;
    }
  }

  saveSettings(settings);

  if (needsRestart == true)
  {
    scheduleRestart();
  }

  request->send(200, "text/plain", "Settings saved successfully");
}

void handleGetSettings(AsyncWebServerRequest *request)
{
  JsonDocument doc;

  doc["wifiSSID"] = settings.wifiSSID;
  // Don't send the actual password for security reasons
  doc["wifiPassword"] = "";
  doc["useNTP"] = settings.useNTP;
  doc["timezone"] = settings.timezone;
  doc["alarmEnabled"] = settings.alarmEnabled;
  doc["alarmTime"] = settings.alarmTime;
  doc["weekdays"].to<JsonArray>();
  for (int i = 0; i < 7; i++)
  {
    doc["weekdays"].add(settings.weekdays[i]);
  }

  String response;
  serializeJson(doc, response);
  request->send(200, "application/json", response);
}
