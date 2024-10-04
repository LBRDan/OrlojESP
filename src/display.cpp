#include <display.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeSerif24pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>

// Time variables
struct tm timeinfo;
char previousTimeStr[9] = "00:00:00";
char previousDateStr[30] = "2021-01-01";

void displayAccessPointInfo(GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> &display, const char *ssid, const char *password)
{
    display.setFullWindow();
    display.setFont(&FreeMono9pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        display.setCursor(10, 30);
        display.println("WiFi Setup Required");
        display.setCursor(10, 60);
        display.println("Connect to WiFi:");
        display.setCursor(10, 90);
        display.println(ssid);
        display.setCursor(10, 120);
        display.println("Password:");
        display.setCursor(10, 150);
        display.println(password);
        display.setCursor(10, 180);
        display.println("Then visit:");
        display.setCursor(10, 210);
        display.println("http://192.168.4.1");
    } while (display.nextPage());
}

void showTimeAndDate(GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> &display, bool fullRefresh)
{
    displayTime(display, fullRefresh);
    displayDate(display, false);
}

void displayTime(GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> &display, bool fullRefresh)
{

    getLocalTime(&timeinfo); // Update time
    char timeStr[9];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);

    if (strcmp(timeStr, previousTimeStr) != 0 || fullRefresh)
    {
        display.setFont(&FreeSerif24pt7b);
        int16_t tbx, tby;
        uint16_t tbw, tbh;
        display.getTextBounds(timeStr, 0, 0, &tbx, &tby, &tbw, &tbh);
        uint16_t x = (display.width() - tbw) / 2;
        uint16_t y = (display.height() + tbh) / 2;
        short margin = 8;

        if (!fullRefresh)
        {
            // display.setPartialWindow(x, y - tbh, tbw, tbh);
            // Add a small margin to the partial update
            display.setPartialWindow(x - margin, y - tbh - margin, tbw + 2 * margin, tbh + 2 * margin);
        }
        display.firstPage();
        do
        {
            display.fillScreen(GxEPD_WHITE);
            if (fullRefresh)
            {
                display.drawRect(0, 0, display.width(), display.height(), GxEPD_BLACK);
                display.drawFastHLine(x, y + 10, tbw + 2 * margin, GxEPD_BLACK);
            }
            display.setCursor(x, y);
            display.print(timeStr);
        } while (display.nextPage());

        strcpy(previousTimeStr, timeStr);
    }
}

void displayDate(GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> &display, bool fullRefresh)
{
    getLocalTime(&timeinfo); // Update time
    char dateStr[30];
    strftime(dateStr, sizeof(dateStr), "%B, %d %b %Y", &timeinfo);

    if (strcmp(dateStr, previousDateStr) == 0)
    {
        return;
    }

    display.setFont(&FreeSerif9pt7b);
    display.setTextColor(GxEPD_BLACK);

    int16_t tbx, tby;
    uint16_t tbw, tbh;
    display.getTextBounds(dateStr, 0, 0, &tbx, &tby, &tbw, &tbh);
    // Calculate X and Y to set the partial window and the cursor at the bottom right corner
    uint16_t x = display.width() - tbw - 10;
    uint16_t y = display.height() - tbh;

    if (!fullRefresh)
    {
        display.setPartialWindow(x, y - tbh, tbw, tbh);
    }

    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        display.setCursor(x, y);
        display.print(dateStr);
    } while (display.nextPage());

    strcpy(previousDateStr, dateStr);
}