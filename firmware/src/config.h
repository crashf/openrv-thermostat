#pragma once
#include <Arduino.h>

// OpenRV Thermostat Configuration
// Pin assignments for ESP32-S3-DevKitC-1

// ============================================================================
// WiFi / Network
// ============================================================================
#define WIFI_STA_SSID       "YOUR_WIFI_SSID"
#define WIFI_STA_PASSWORD   "YOUR_WIFI_PASSWORD"
#define WIFI_AP_SSID        "OpenRV-Setup"
#define WIFI_AP_PASSWORD    "openrvcamp"

#define HOSTNAME            "openrv-thermostat"   // mDNS hostname
#define OTA_PASSWORD        "openrv"            // OTA update password
#define WEB_SERVER_PORT     80

// ============================================================================
// 6-Wire RV Harness Mapping
// ============================================================================
// Red   = +12V input (power supply)
// Blue  = Ground
// White = Furnace control (switched +12V out)
// Yellow= AC Compressor (switched +12V out)
// Green = Fan High (switched +12V out)
// Grey  = Fan Low (switched +12V out)

// Control output pins (active HIGH = relay ON)
#define PIN_FURNACE         5    // White wire
#define PIN_AC_COMPRESSOR   6    // Yellow wire
#define PIN_FAN_HI          7    // Green wire
#define PIN_FAN_LO          8    // Grey wire

// Power is Red (+12V in) and Blue (GND) — handled by external buck converter

// ============================================================================
// Temperature Sensors
// ============================================================================
// On-board DS18B20 (1-Wire bus)
#define PIN_ONE_WIRE        4
#define DS18B20_RESOLUTION  10   // 9-12 bits (higher = slower, more accurate)

// Expansion I2C bus (for future remote sensors: SHT30, AHT20, BME280)
#define PIN_I2C_SDA         3
#define PIN_I2C_SCL         2

// ============================================================================
// TFT Display (ILI9341 + XPT2046 Resistive Touch, SPI bus)
// ============================================================================
#define TFT_PIN_CS          10
#define TFT_PIN_DC          11
#define TFT_PIN_RST         12
#define TFT_PIN_MOSI        13
#define TFT_PIN_MISO        14
#define TFT_PIN_SCK         15

#define TOUCH_PIN_CS        16
#define TOUCH_PIN_IRQ       17

// Display config (set in TFT_eSPI User_Setup.h or via build flags)
#define TFT_WIDTH           240
#define TFT_HEIGHT          320
#define TFT_ROTATION        1    // Landscape (0=portrait, 1-3=landscape variants)

// ============================================================================
// Status LEDs (optional on v0.2 PCB)
// ============================================================================
#define LED_PIN_POWER       37   // On-board LED near power input
#define LED_PIN_STATUS      38   // Status / WiFi activity LED

// ============================================================================
// Pushbuttons (optional on v0.2 PCB)
// ============================================================================
#define BTN_PIN_RESET       0    // ESP32 BOOT button (GPIO0)
#define BTN_PIN_USER        18   // User-configurable button

// ============================================================================
// Thermostat Logic
// ============================================================================
// Temperature bounds and defaults
#define TEMP_MIN_F          55.0
#define TEMP_MAX_F          95.0
#define TEMP_DEFAULT_F      72.0
#define TEMP_HYSTERESIS_F   0.5  // Deadband to prevent rapid cycling

// Mode definitions (C-compatible enum)
enum ThermoMode {
    MODE_OFF = 0,
    MODE_FAN_LO = 1,
    MODE_FAN_HI = 2,
    MODE_COOL = 3,
    MODE_HEAT = 4,
    MODE_AUTO = 5
};

// Fan modes
enum FanMode {
    FAN_AUTO = 0,
    FAN_LO   = 1,
    FAN_HI   = 2
};

// ============================================================================
// Timing
// ============================================================================
#define SENSOR_READ_INTERVAL_MS     1000   // 1 second
#define CONTROL_LOOP_INTERVAL_MS    5000   // 5 seconds
#define WEB_CLIENT_TIMEOUT_MS       5000
#define OTA_CHECK_INTERVAL_MS       3600000L  // 1 hour (OTA availability check)

// ============================================================================
// Debug
// ============================================================================
// #define DEBUG_SERIAL          Serial
// Uncomment to enable verbose logging via USB serial
// Production builds should define NDEBUG to strip debug code
