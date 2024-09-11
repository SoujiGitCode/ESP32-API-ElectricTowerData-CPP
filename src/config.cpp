#include "config.h"
#include "functions.h"
#include "nvs_flash.h"

Config::Config()
{
    Serial.println("Config Constructor");
    preferences.begin("my-app", false);
    // preferences.clear();
    //  Check if userConfig exists in preferences
    if (!preferences.isKey("userConfig"))
    {
        // If userConfig doesn't exist, we initialize factoryConfig to true
        // and userConfig to false
        Serial.println("Initializing userConfig to false and factoryConfig to true");
        preferences.putBool("factoryConfig", true);
        preferences.putBool("userConfig", false);
    }

    // Check if ssid and password are configured
    if (!preferences.getString("ssid").length())
    {
        preferences.putString("ssid", defaultValues.ssid);
    }
    if (!preferences.getString("password").length())
    {
        preferences.putString("password", defaultValues.password);
    }

    // If userConfig is true, this means user has already configured the settings
    // so we use the user configured factoryConfig value
    if (preferences.getBool("userConfig", false))
    {
        currentConfig = false;
    }
    else
    {
        currentConfig = true;
    }
    Serial.print("At startup, userConfig is: ");
    Serial.println(preferences.getBool("userConfig", false));
    Serial.print("At startup, factoryConfig is: ");
    Serial.println(preferences.getBool("factoryConfig", true));
    preferences.end();
}

// void Config::resetToFactoryDefaults()
// {
//     preferences.begin("my-app", false);
//     preferences.putBool("factoryConfig", true);
//     preferences.putBool("userConfig", false);
//     preferences.end();
// }
void Config::resetToFactoryDefaults()
{
    // Borra toda la memoria NVS y reiníciala
    esp_err_t err = nvs_flash_erase();
    if (err == ESP_OK)
    {
        Serial.println("NVS flash erased successfully.");
    }
    else
    {
        Serial.println("Failed to erase NVS flash.");
    }

    err = nvs_flash_init();
    if (err == ESP_OK)
    {
        Serial.println("NVS flash initialized successfully.");
    }
    else
    {
        Serial.println("Failed to initialize NVS flash.");
    }

    // Restablecer valores predeterminados en 'my-app'
    preferences.begin("my-app", false);
    preferences.putBool("factoryConfig", true);
    preferences.putBool("userConfig", false);
    preferences.end();

    // Restablecer el flag 'defaultValues' en 'cells_Namespace'
    preferences.begin("cells_Namespace", false);
    preferences.putBool("defaultValues", true); // Aseguramos que se inicialicen los valores por defecto
    preferences.end();

    Serial.println("Factory reset completed.");
}

void Config::noMoreFactoryValues()
{
    preferences.begin("my-app", false);
    preferences.putBool("factoryConfig", false);
    preferences.putBool("userConfig", true);
    Serial.println("Just set userConfig to true and factoryConfig to false");
    Serial.print("userConfig is now: ");
    Serial.println(preferences.getBool("userConfig", false));
    Serial.print("factoryConfig is now: ");
    Serial.println(preferences.getBool("factoryConfig", true));
    preferences.end();
}

void Config::setSsid(const char *ssid)
{
    preferences.begin("my-app", false);
    Serial.println("Setting SSID");
    preferences.putString("ssid", ssid);
    ssidStr = ssid;
    noMoreFactoryValues();
    preferences.end();
}

void Config::setPassword(const char *password)
{
    preferences.begin("my-app", false);
    Serial.println("Setting Password");
    preferences.putString("password", password);
    passStr = password;
    noMoreFactoryValues();
    preferences.end();
}

const char *Config::getSsid()
{
    preferences.begin("my-app", false);
    if (preferences.getBool("factoryConfig", true))
    {
        preferences.end();
        return defaultValues.ssid;
    }
    else
    {
        ssidStr = preferences.getString("ssid");
        preferences.end();
        return ssidStr.c_str();
    }
}

const char *Config::getPassword()
{
    preferences.begin("my-app", false);
    if (preferences.getBool("factoryConfig", true))
    {
        preferences.end();
        return defaultValues.password;
    }
    else
    {
        passStr = preferences.getString("password");
        preferences.end();
        return passStr.c_str();
    }
}

void Config::setCurrentConfig(bool value)
{
    preferences.begin("my-app", false);
    currentConfig = value;
    preferences.putBool("factoryConfig", true);
    preferences.putBool("userConfig", value);
    preferences.end();
}

bool Config::getCurrentConfig()
{
    return currentConfig;
}

void Config::setFactoryResetRequest(bool value)
{
    preferences.begin("my-app", false);
    currentConfig = true;
    preferences.putBool("factoryConfig", false);
    preferences.putBool("userConfig", true);
    preferences.end();
}

bool Config::getFactoryResetRequest()
{
    return factoryResetRequest;
}

// Tower Info

// Implementación de setTowerInfo
void Config::setTowerInfo(const TowerInfo &info)
{
    String json = serializeTowerInfo(info); // Utiliza la función serializeTowerInfo
    preferences.begin("my-app", false);
    preferences.putString("towerInfo", json);
    preferences.end();
}

// Implementación de getTowerInfo
TowerInfo Config::getTowerInfo()
{
    preferences.begin("my-app", false);
    String json = preferences.getString("towerInfo", "");
    preferences.end();

    if (json.length() > 0)
    {
        return deserializeTowerInfo(json); // Utiliza la función deserializeTowerInfo
    }
    else
    {
        return TowerInfo(); // Retorna un objeto vacío si no hay datos guardados
    }
}