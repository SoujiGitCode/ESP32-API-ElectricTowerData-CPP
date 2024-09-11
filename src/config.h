#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <ArduinoJson.h>
#include <Preferences.h>
#include "functions.h"

struct DefaultValues
{
    const char *ssid = "selydefault";
    const char *password = "";
};

class Config
{
private:
    Preferences preferences;
    DefaultValues defaultValues;
    bool factoryResetRequest = false;
    bool currentConfig = false;

    String ssidStr;
    String passStr;

public:
    Config();
    void setSsid(const char *ssid);
    void setPassword(const char *password);
    const char *getSsid();
    const char *getPassword();
    void setCurrentConfig(bool value);
    bool getCurrentConfig();
    bool getFactoryResetRequest();
    void setFactoryResetRequest(bool value);
    void resetToFactoryDefaults();
    void noMoreFactoryValues();

    void setTowerInfo(const TowerInfo &info);
    TowerInfo getTowerInfo();
};

extern Config config;

#endif // _CONFIG_H_
