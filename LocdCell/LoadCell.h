#pragma once
#include "HX711.h"
#define DOUT    (2)
#define SCK     (3)


class LoadCell {
public:
    //HX711 scale;
    LoadCell(void) {
    }
    ~LoadCell() {
    }
    // void initScale(void) {
    //     HX711 scale;
    //     scale.begin(DOUT,SCK);
    // }
    int getLoadValue(void) {
        HX711 scale;
        scale.begin(DOUT,SCK);
        if(scale.is_ready()) {
            long reading = scale.read();
            return reading;
            // Serial.print(reading);
        }
        // else {
        //     Serial.println("HX711 not found");
        // }
    }
    String getLoadValueStr(int nPrintPrec) {
        return String(getLoadValue(), nPrintPrec);
    }
protected:
};