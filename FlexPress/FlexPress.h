#pragma once
#define PINFLEXPRS_L    (36)
#define PINFLEXPRS_R    (35)
#define PINPRS_T        (39)

class FlexPress {
    public:
    FlexPress(void) {
    }
    ~FlexPress() {
    }

    void initFlex(void) {
        pinMode(PINFLEXPRS_L, INPUT);
        pinMode(PINFLEXPRS_R, INPUT);
        pinMode(PINPRS_T, INPUT);
    }
    int getFlexPressL(void) {
        return analogRead(PINFLEXPRS_L);
    }
    int getFlexPressR(void) {
        return analogRead(PINFLEXPRS_R);
    }
    int getPressT(void) {
        return analogRead(PINPRS_T);
    }
    int getFlexPressMapL(void) {
        return map(getFlexPressL(), 0, 1023, 0, 255);
    }
    int getFlexPressMapR(void) {
        return map(getFlexPressR(), 0, 1023, 0, 255);
    }
    int getPrsMap1(void) {
        return map(getPressT(), 0, 1023, 0, 255);
    }
    String getFlexPressStrL(int nPrintPrec) {
        return String(getFlexPressL(), nPrintPrec);
    }
    String getFlexPressStrR(int nPrintPrec) {
        return String(getFlexPressR(), nPrintPrec);
    }
    String getPressStrT(int nPrintPrec) {
        return String(getPressT(), nPrintPrec);
    }

    protected:
};