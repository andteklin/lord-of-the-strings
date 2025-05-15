/* buttons.c
Implementations of four buttons pins
*/
#include "buttons.h"
#include "strummer.h"
#include "MKL46Z4.h"

// — pin definitions by port —
static const uint8_t portA_pins[] = { 1,  2 };  // tempo up/down
static const uint8_t portC_pins[] = { 0     };  // mute
static const uint8_t portD_pins[] = { 3     };  // style
extern volatile MuteState muteState;

void Buttons_Init(void) {
    // 1) enable clocks for PORTA, PORTC, PORTD
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK
                |  SIM_SCGC5_PORTC_MASK
                |  SIM_SCGC5_PORTD_MASK;

    // 2) configure PTA1, PTA2 for falling‑edge IRQ, pull‑up
    for (int i = 0; i < sizeof(portA_pins)/sizeof(portA_pins[0]); i++) {
        uint8_t pin = portA_pins[i];
        PORTA->PCR[pin] = PORT_PCR_MUX(1)      // GPIO
                        | PORT_PCR_PE_MASK    // pull enabled
                        | PORT_PCR_PS_MASK    // pull‑up
                        | PORT_PCR_IRQC(0xA); // falling‑edge
    }
    // 3) configure PTC0
    PORTC->PCR[ portC_pins[0] ] = PORT_PCR_MUX(1)
                                | PORT_PCR_PE_MASK
                                | PORT_PCR_PS_MASK
                                | PORT_PCR_IRQC(0xA);
    // 4) configure PTD3
    PORTD->PCR[ portD_pins[0] ] = PORT_PCR_MUX(1)
                                | PORT_PCR_PE_MASK
                                | PORT_PCR_PS_MASK
                                | PORT_PCR_IRQC(0xA);
    // Make PTA1 and PTA2 inputs
    GPIOA->PDDR &= ~(1u << 1);  // PTA1 = input
    GPIOA->PDDR &= ~(1u << 2);  // PTA2 = input

    // Make PTC0 input
    GPIOC->PDDR &= ~(1u << 0);  // PTC0 = input

    // Make PTD3 input
    GPIOD->PDDR &= ~(1u << 3);  // PTD3 = input

    // 5) enable NVIC for each port
    NVIC_ClearPendingIRQ(PORTA_IRQn);
    NVIC_EnableIRQ    (PORTA_IRQn);

    NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);
    NVIC_EnableIRQ    (PORTC_PORTD_IRQn);
}

void PORTA_IRQHandler(void) {
    // PTA1 (BTN_TEMPO_UP), PTA2 (BTN_TEMPO_DOWN)
    for (int i = 0; i < 2; i++) {
        uint8_t pin = portA_pins[i];
        if (PORTA->PCR[pin] & PORT_PCR_ISF_MASK) {
            PORTA->PCR[pin] |= PORT_PCR_ISF_MASK;  // clear flag
            if (i == 0) {
                // BTN_TEMPO_UP
            } else {
                // BTN_TEMPO_DOWN
            }
        }
    }
}

void PORTC_PORTD_IRQHandler(void) {
    // PTC0 (BTN_MUTE_TOGGLE)
    uint8_t pinC = portC_pins[0];
    if (PORTC->PCR[pinC] & PORT_PCR_ISF_MASK) {
        PORTC->PCR[pinC] |= PORT_PCR_ISF_MASK;  // clear flag
        strummer_toggleMute();
    }
    // PTD3 (BTN_STYLE_CYCLE)
    uint8_t pinD = portD_pins[0];
    if (PORTD->PCR[pinD] & PORT_PCR_ISF_MASK) {
        PORTD->PCR[pinD] |= PORT_PCR_ISF_MASK;  // clear flag
        // BTN_STYLE_CYCLE
    }
}
