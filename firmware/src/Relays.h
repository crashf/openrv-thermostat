#pragma once
#include <Arduino.h>
#include "config.h"

class Relays {
public:
    Relays() {}
    void init();
    void setFurnace(bool on);
    void setCompressor(bool on);
    void setFanHi(bool on);
    void setFanLo(bool on);
    
    bool getFurnace() const { return furnaceOn; }
    bool getCompressor() const { return compressorOn; }
    bool getFanHi() const { return fanHiOn; }
    bool getFanLo() const { return fanLoOn; }
    
    // Safety: force all off
    void emergencyOff();

private:
    bool furnaceOn = false;
    bool compressorOn = false;
    bool fanHiOn = false;
    bool fanLoOn = false;
};
