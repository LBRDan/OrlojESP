# OrlojESP

A "smart" ESP32 based 2.9 inches e-paper bedside clock with 24 hours ahead weather forecast

Target features:
- [x] Configurable WiFi connection via AP mode
- [x] Time and date display
- [x] Time and date synchronization via NTP (with timezone support) when connected to WiFi
- [-] Web interface for configuration
- [ ] Front light
- [ ] 24 hours ahead Weather forecast (with weather alerts)
- [ ] Lithium battery powered (with charging control and low battery signal, battery level display and load sharing)
- [ ] Battery powered RTC module


## Hardware {WIP}

- ESP32
- 2.9 inches e-paper display (WeArt 2.9 inch e-paper display module)
- 3.7V 500mAh LiPo battery
- 3.7V LiPo battery charger
- DS3231 RTC module
- Boost converter (3.3V to 5V if needed)
- Buck converter (3.7V to 3.3V)

## Dependencies
Built with PlatformIO and Visual Studio Code
