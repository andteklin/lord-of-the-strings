/*
    pwm.c
    For testing one single servo motor with PWM
*/
#include "pwm.h"
#include <MKL46Z4.h>
#include <stdint.h>
#include <init.c>

// // Constants for muting
// #define STRUM_START angle_to_cnV(60)
// #define STRUM_END   angle_to_cnV(120)
// #define MUTE_ON     angle_to_cnV(150)
// #define MUTE_OFF    angle_to_cnV(30)

uint16_t angle_to_cnV(uint8_t angle) {
    const uint16_t minTicks = 753;   // 1 ms (0.02 * 15075)
    const uint16_t maxTicks = 1508;   // 2 ms (0.04 * 15075)
    return minTicks + (angle * (maxTicks - minTicks)) / 180;
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
    
    // // Crystal Oscillator clock
    // MCG->C2 |= MCG_C2_EREFS0_MASK; // Enable crystal oscillator
    // OSC0->CR |= OSC_CR_ERCLKEN_MASK; // Enable oscillator output
    // MCG->C2 = MCG_C2_RANGE(2) | MCG_C2_EREFS_MASK; // High-frequency range, external reference

    // // Configure FLL for 48 MHz
    // MCG->C1 = MCG_C1_CLKS(0) | MCG_C1_FRDIV(3) | MCG_C1_IREFS(0); // External reference, FLL engaged
    // MCG->C4 |= MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(1); // DCO range for 48 MHz

    // // Wait for FLL to lock
    // while (!(MCG->S & MCG_S_IREFST_MASK)) {}
    // while ((MCG->S & MCG_S_CLKST_MASK) != MCG_S_CLKST(0)) {}

    SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK; // Clear existing bits
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(0b01); // MCGFLLCLK (1) as TPM clock source, or external oscillator (10)
    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL(0); 
    // TPM clock of 20.97MHz with Prescaler 128 gives 163.84 KHz input clock
    // 20 ms period gives 7500 value for modulo register
    TPM2->SC = 0; // Disable TPM
    // TPM2->MOD = 1249; // 20 ms period with 62.5kHz clock
    // TPM2->MOD = 7494; // 20ms period with 48MHz/128 prescaler
    TPM2->MOD = 15075; // 20ms period with 48Mhz/64 Prescaler, 747 ticks/ms
    // TPM2->MOD = 3276; // 20 ms period with 163.84 KHz TPM

    // Enable Edge-Aligned PWM with high-true output on TPM2 Channels
    TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;  // Edge PWM CH0
    TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;  // Edge PWM CH1

    // Default Servo Pulse Width
    TPM2->CONTROLS[0].CnV = angle_to_cnV(90);
    TPM2->CONTROLS[1].CnV = angle_to_cnV(90);
    
    // Start timer with prescaler = 64, start timer, clear timer overflow flag
    TPM2->SC = TPM_SC_PS(6) | TPM_SC_CMOD(1) | TPM_SC_TOF_MASK;
}

void straight() {
    TPM2->CONTROLS[0].CnV = angle_to_cnV(90);
}
void turnCW90() {
    TPM2->CONTROLS[0].CnV = angle_to_cnV(180);
}
void turnCCW90() {}