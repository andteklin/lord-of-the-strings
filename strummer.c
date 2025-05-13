/* strummer.c
    Implementations of functions in strummer.h
*/
#include "strummer.h"
#include "pwm.h"   // assumes you have angle_to_cnV() here
#include "MKL46Z4.h"

extern volatile uint32_t millis;
// For 120 BPM quarter note → 500 ms per beat
// strumDeadline = millis + 500;
// bool strumming = true;
// if (strumming);

typedef enum {
    STRUM_DOWN,
    STRUM_WAIT_1,
    STRUM_UP,
    STRUM_WAIT_2
} StrumState;

static StrumState strumState = STRUM_DOWN;
static uint32_t strumDeadline = 0;
static bool strumming_enabled = false;

void strummer_enable(bool on) {
    strumming_enabled = on;
    if (on) {
        strumState = STRUM_DOWN;
    }
}

void strummer_init(void) {
    // Assumes TPM2 and servo channel already configured
    TPM2->CONTROLS[0].CnV = angle_to_cnV(90); // Neutral starting position
}

void strummer_update(void) {
    if (!strumming_enabled) return;

    switch (strumState) {
        case STRUM_DOWN:
            TPM2->CONTROLS[0].CnV = angle_to_cnV(60);  // downstroke
            strumDeadline = millis + 200;              // hold for 200ms
            strumState = STRUM_WAIT_1;
            break;

        case STRUM_WAIT_1:
            if (millis >= strumDeadline) {
                strumState = STRUM_UP;
            }
            break;

        case STRUM_UP:
            TPM2->CONTROLS[0].CnV = angle_to_cnV(120); // upstroke
            strumDeadline = millis + 200;
            strumState = STRUM_WAIT_2;
            break;

        case STRUM_WAIT_2:
            if (millis >= strumDeadline) {
                strumState = STRUM_DOWN; // repeat
            }
            break;
    }
}
