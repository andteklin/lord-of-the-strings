/* strummer.c
    Implementations of functions in strummer.h
*/
#include "strummer.h"
#include "pwm.h" 
#include "MKL46Z4.h"

volatile uint32_t millis = 0;
volatile uint8_t step_idx = 0;
volatile bool looping = 0;
// For 120 BPM quarter note → 500 ms per beat
// strumDeadline = millis + 500;
// bool strumming = true;
// if (strumming);

static StrumState strumState;
// static uint32_t strumDeadline = 0;
static volatile bool strumming_enabled = false;
#define TICK_MS 10
#define NUM_STEPS 1

/* :| _ ^ _ ^ _ ^ _ ^ _^ :| */
Step schedule[NUM_STEPS] = {
    {0, STRUM_UP},
    // {20, STRUM_UP},
    // {160,  STRUM_UP},
    // {240, STRUM_DOWN},
    // {320,  STRUM_UP},
    // {400, STRUM_DOWN},
    // {480, STRUM_UP},
    // {560, STRUM_DOWN},
    // {640, STRUM_UP},
    // {680, STRUM_DOWN},
    // {720, STRUM_UP}
};

// delay( c * time_base ) 
/* Start or stop the schedule */
void strummer_enable(bool on) {
    strumming_enabled = on;
    millis = 0;
    step_idx = 0;
    if (on) {
        strumState = schedule[0].state;
    }
}

void strummer_init(void) {
    // Assumes TPM2 and servo channel already configured
    TPM2->CONTROLS[0].CnV = angle_to_cnV(90); // Neutral starting position
}

/* update the state of the servos */
void state_update() {
    if (!strumming_enabled) return;

    switch (strumState) {
        case STRUM_DOWN:
            TPM2->CONTROLS[0].CnV = angle_to_cnV(0);
            break;
        case STRUM_UP:
            TPM2->CONTROLS[0].CnV = angle_to_cnV(360);
            break;
        
        case MUTE_ON:
            TPM2->CONTROLS[1].CnV = angle_to_cnV(120);
            break;
        case MUTE_OFF:
            TPM2->CONTROLS[1].CnV = angle_to_cnV(90);
            break;
    }
}

/* called by PIT Interrupt Service Routine */
void strummer_update(void) {
    millis += TICK_MS;
    if (step_idx < NUM_STEPS && millis >= schedule[step_idx].time_ms) {
        state_update();
        step_idx++;
        if ((step_idx >= NUM_STEPS)) {
            step_idx = 0;
            millis = 0;
            strumState = schedule[step_idx].state;
            // strumming_enabled = false;
        }
        else {
            strumState = schedule[step_idx].state;
        }
    }
}

/* Initialize PIT timer */
void PIT_Init(uint32_t tick_ms) {
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; // Enable clock to PIT module
    PIT->MCR = 0; //Make sure the peripheral is enabled, should be on by default. 
    PIT->CHANNEL[0].LDVAL = (SystemCoreClock/1000 * tick_ms) - 1; // Set timer period, about 1/10 sec
    PIT->CHANNEL[0].TFLG = 1; // clear previous interrupts
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; // enable countdown (Timer Interrupt Enable, Timer enable)
    NVIC_EnableIRQ(PIT_IRQn); // writes to ISER and ICER registers
}

/* interrupt every TICK_MS ms */
void PIT_IRQHandler(void) {
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
    strummer_update();
}
