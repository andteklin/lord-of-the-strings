/* strummer.c
    Implementations of functions in strummer.h
*/
#include "strummer.h"
#include "board.h"
#include "pwm.h"
#include "patterns.h"
#include "MKL46Z4.h"

#define TICK_MS 10 // interrupt intervals for PIT
// volatile uint32_t ms_counter = 0; // for the timestamp-based scheduler
volatile uint32_t remaining_ms; // to track current time remaining
volatile uint8_t step_idx = 0; // to track which step in pattern we are in
volatile bool loop = 0; // by default no looping of pattern

// Pattern definitions
static const PatternStep* curPattern;
static uint8_t curLen; // length of current pattern

// current strumming state
static StrumState strumState;
// current muting state
static StrumState muteState;
// enable strumming
static volatile bool strumming_enabled = 0;

// Tempo and base durations
#define BPM                120                      // beats per minute
#define MS_PER_BEAT        (60000 / BPM)           // ms in one quarter‑note
#define MS_PER_SIXTEENTH   (MS_PER_BEAT / 4)       // ms in a 16th‑note

// Servo Angle definitions
#define SU 0 // strumming up angle
#define SD 180 // strumming down angle
#define MU 0 // angle for mute bar up
#define MD 0 // angle for mute bar down

void strummer_init(void) {
    BOARD_InitBootClocks();
    BOARD_BootClockRUN(); // Configure FLL for 48MHz
    SystemCoreClockUpdate();
    setupPWM();
    PIT_Init(); // initialize PIT timer 10ms
    // Assumes TPM2 and servo channel already configured
    TPM2->CONTROLS[0].CnV = angle_to_cnV(90); // Neutral starting position
}

/* update the state of the servos */
void state_update(void) {
    switch (strumState) {
        case STRUM_DOWN:
            TPM2->CONTROLS[0].CnV = angle_to_cnV(SD);
            break;
        case STRUM_UP:
            TPM2->CONTROLS[0].CnV = angle_to_cnV(SU);
            break;
        
        case MUTE_ON:
            TPM2->CONTROLS[1].CnV = angle_to_cnV(MD);
            break;
        case MUTE_OFF:
            TPM2->CONTROLS[1].CnV = angle_to_cnV(MU);
            break;
        case REST:
            break;
    }
}

/* Start or stop the pattern/schedule */
void strummer_enable(bool doLoop, PatternId pid) {
    strumming_enabled = true;
    loop = doLoop;
    // ms_counter = 0;
    // set current pattern pointer
    curPattern = Patterns_Get(pid);
    curLen = Patterns_Len(pid);

    step_idx = 0;
    remaining_ms = curPattern[0].length * MS_PER_SIXTEENTH;
    // strumState = schedule[0].state;
    strumState = curPattern[0].state;
    state_update();
}

/* called by PIT Interrupt Service Routine */
void strummer_update(void) {
    if (!strumming_enabled) return;
    // ms_counter += TICK_MS;
    // For use with PatternStep
    // If there is still time remaining greater than PIT interval, subtract PIT interval
    if (remaining_ms > TICK_MS) {
        remaining_ms -= TICK_MS;
        return;
    }
    // Advance to next step after remaining ms runs out
    step_idx++;
    if (step_idx >= curLen) {
        if (loop) {
            step_idx = 0;
        }
        else {
            strumming_enabled = false;
            return;
        }
    }

    // load new step
    remaining_ms = curPattern[step_idx].length * MS_PER_SIXTEENTH;
    strumState = curPattern[step_idx].state;
    state_update();
    // // For use with Step scheduler
    // if (step_idx < NUM_STEPS && ms_counter >= schedule[step_idx].time_ms) {
    //     state_update();
    //     step_idx++;
    //     if ((step_idx >= NUM_STEPS)) {
    //         step_idx = 0;
    //         ms_counter = 0;
    //         strumState = schedule[step_idx].state;
    //         // strumming_enfabled = false;
    //     }
    //     else {
    //         strumState = schedule[step_idx].state;
    //     }
    // }
}

/* Initialize PIT timer */
void PIT_Init() {
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; // Enable clock to PIT module
    PIT->MCR = 0; //Make sure the peripheral is enabled, should be on by default. 
    PIT->CHANNEL[0].LDVAL = (SystemCoreClock/1000 * TICK_MS) - 1; // Set timer period, about 1/10 sec
    PIT->CHANNEL[0].TFLG = 1; // clear previous interrupts
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; // enable countdown (Timer Interrupt Enable, Timer enable)
    NVIC_EnableIRQ(PIT_IRQn); // writes to ISER and ICER registers
}

/* interrupt every TICK_MS ms */
void PIT_IRQHandler(void) {
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
    strummer_update();
}
