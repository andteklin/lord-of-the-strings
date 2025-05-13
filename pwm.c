/*
    pwm.c
    For testing one single servo motor with PWM
*/
#include "pwm.h"
#include <MKL46Z4.h>
#include <stdint.h>
#include <init.c>

uint16_t angle_to_cnV(uint8_t angle) {
    // angle: 0–180
    return 63 + (angle * (125 - 63)) / 180;
}

/*
    Setup the pins and timing.
*/
void setup(void) {
    // Pin Setup
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;           // Enable Port B clock
    // For Servo 1 (Strummer)
    PORTB->PCR[18] &= ~PORT_PCR_MUX_MASK; // clear mux bits
    PORTB->PCR[18] |= PORT_PCR_MUX(3);            // ALT3 = TPM2_CH0
    // For Servo 2 (Muter)
    PORTB->PCR[19] &= ~PORT_PCR_MUX_MASK; // clear mux bits
    PORTB->PCR[19] |= PORT_PCR_MUX(3); // for TPM2_CH1
    
    // Timing Setup
    SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK; // Enable clock for TPM2
    
    // Crystal Oscillator clock
    MCG->C2 |= MCG_C2_EREFS0_MASK; // Enable crystal oscillator
    OSC0->CR |= OSC_CR_ERCLKEN_MASK; // Enable oscillator output
    
    SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK; // Clear existing bits
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(0b10); // select exteranl oscillator crystal on 0b10

    // TPM clock of 8MHz with Prescaler 128 gives 62.5 KHz input clock
    // 20 ms period gives 1250 value for modulo register
    TPM2->SC = 0; // Disable TPM
    TPM2->MOD = 1249; // 20ms period

    // Enable Edge-Aligned PWM with high-true output on TPM2 Channels
    TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;  // Edge PWM CH0
    TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;  // Edge PWM CH1

    // Default Servo Pulse Width
    TPM2->CONTROLS[0].CnV = angle_to_cnV(90);
    TPM2->CONTROLS[1].CnV = angle_to_cnV(94);

    TPM2->SC |= TPM_SC_TOF_MASK; // Clear Timer Overflow flag
    
    // Start timer with prescaler = 128, start timer
    TPM2->SC = TPM_SC_PS(7) | TPM_SC_CMOD(1);
}

void straight() {
    TPM2->CONTROLS[0].CnV = angle_to_cnV(90);
}
void turnCW90() {
    TPM2->CONTROLS[0].CnV = angle_to_cnV(45);
}
void turnCCW90() {}