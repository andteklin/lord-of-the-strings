/* buttons.c
Implementations of four buttons pins
*/

#include "buttons.h"
#include "MKL46Z4.h"

// ── Configure these to the pins you wired your buttons to ────────────────
// Here we use PTA1, PTA2, PTA4, PTA5 as examples:
static const uint32_t buttonPins[4] = {
    1,  // button 0 → PTA1
    2,  // button 1 → PTA2
    4,  // button 2 → PTA4
    5   // button 3 → PTA5
};

// simple debounce state
static bool lastState[4];
static bool pressedFlag[4];

void Buttons_Init(void) {
    // 1) Enable clock to PORTA
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    // 2) Configure each pin as GPIO input with pull‑up
    for (int i = 0; i < 4; i++) {
        uint32_t pin = buttonPins[i];
        PORTA->PCR[pin] = PORT_PCR_MUX(1)    // GPIO
                        | PORT_PCR_PE_MASK  // pull enable
                        | PORT_PCR_PS_MASK; // pull‑up
    }
    // 3) Clear any pending and enable PORTA IRQ in NVIC
    NVIC_ClearPendingIRQ(PORTA_IRQn);
    NVIC_EnableIRQ(PORTA_IRQn);
}

void PORTA_IRQHandler(void) {
    for (int i = 0; i < NUM_BUTTONS; i++) {
        uint8_t pin = buttonPins[i];
        if (PORTA->PCR[pin] & PORT_PCR_ISF_MASK) {
            PORTA->PCR[pin] |= PORT_PCR_ISF_MASK;  // clear flag

            // dispatch your action
            switch (i) {
              case BTN_TEMPO_UP:
                // increase BPM, recalc timings
                break;
              case BTN_TEMPO_DOWN:
                // decrease BPM
                break;
              case BTN_MUTE_TOGGLE:
                // toggle mute in your strummer module
                break;
              case BTN_STYLE_CYCLE:
                // advance to next strum pattern
                break;
            }
        }
    }
}
