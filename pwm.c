/*
    pwm.c
    For testing one single servo motor with PWM
*/
#include <MKL46Z4.h>
#include <init.c>

/*
    Setup the pins and timing.
*/
void setup() {
    // Pin Setup
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;           // Enable Port B clock
    // For Servo 1 (Strummer)
    PORTB->PCR[18] &= ~PORT_PCR_MUX_MASK; // clear mux bits
    PORTB->PCR[18] |= PORT_PCR_MUX(3);            // ALT3 = TPM2_CH0
    // For Servo 2 (Muter)
    PORTB->PCR[19] &= ~PORT_PCR_MUX_MASK; // clear mux bits
    PORTB->PCR[19] |= PORT_PCR_MUX(3); // for TPM2_CH0

    // Timing Setup
    
}

void straight() {}
void turnCW90() {}
void turnCCW90() {}