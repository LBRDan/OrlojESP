#include <GxEPD2_BW.h>

void displayAccessPointInfo(GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> &display, const char *ssid, const char *password);
void displayTime(GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> &display, bool fullRefresh);
void displayDate(GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> &display, bool fullRefresh);
void showTimeAndDate(GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> &display, bool fullRefresh);