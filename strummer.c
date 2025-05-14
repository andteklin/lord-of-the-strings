/* strummer.c
    Implementations of functions in strummer.h
*/
#include "strummer.h"
#include "pwm.h" 
#include "MKL46Z4.h"

#define TICK_MS 10
volatile uint32_t ms_counter = 0;
volatile uint32_t remaining_ms;
volatile uint8_t step_idx = 0;
volatile bool loop = 1;
// For 120 BPM quarter note → 500 ms per beat
// strumDeadline = ms_counter + 500;
// bool strumming = true;
// if (strumming);

static StrumState strumState;
// static uint32_t strumDeadline = 0;
static volatile bool strumming_enabled = true;

// Tempo and base durations
#define BPM                60                      // beats per minute
#define MS_PER_BEAT        (60000 / BPM)           // ms in one quarter‑note
#define MS_PER_SIXTEENTH   (MS_PER_BEAT / 4)       // ms in a 16th‑note

// Servo Angle definitions
#define SU 90
#define SD 135
#define MU 0
#define MD 0

// /* :| _ ^ _ ^ _ ^ _ ^ _^ :| */
// Step schedule[NUM_STEPS] = {
//     {0, STRUM_UP},
//     {20, STRUM_UP}
//     // {160,  STRUM_UP},
//     // {240, STRUM_DOWN},
//     // {320,  STRUM_UP},
//     // {400, STRUM_DOWN},
//     // {480, STRUM_UP},
//     // {560, STRUM_DOWN},
//     // {640, STRUM_UP},
//     // {680, STRUM_DOWN},
//     // {720, STRUM_UP}
// };

// Example “down–up–down–up” on 16ths, then rest for an 8th:
static const PatternStep pattern[] = {
    { STRUM_DOWN, DUR_4 },
    { STRUM_UP,   DUR_8 },
    { STRUM_DOWN, DUR_8 },
    { REST,       DUR_8  },
    { STRUM_UP,   DUR_8 },
    { STRUM_DOWN, DUR_8 },
    { STRUM_UP,   DUR_8 },
};
#define PATTERN_LEN  (sizeof pattern / sizeof pattern[0])

void strummer_init(void) {
    BOARD_InitBootClocks();
    BOARD_BootClockRUN(); // Configure FLL for 48MHz
    SystemCoreClockUpdate();
    setup();
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

/* Start or stop the schedule */
void strummer_enable(bool on) {
    strumming_enabled = on;
    ms_counter = 0;
    step_idx = 0;
    remaining_ms = pattern[0].length * MS_PER_SIXTEENTH;
    if (on) {
        // strumState = schedule[0].state;
        strumState = pattern[0].state;
        state_update();
    }
}

/* called by PIT Interrupt Service Routine */
void strummer_update(void) {
    if (!strumming_enabled) return;
    ms_counter += TICK_MS;
    // For use with PatternStep
    if (remaining_ms > TICK_MS) {
        remaining_ms -= TICK_MS;
        return;
    }
    // Advance to next step after remaining ms runs out
    step_idx++;
    if (step_idx >= PATTERN_LEN) {
        if (loop) {
            step_idx = 0;
        }
        else {
            strumming_enabled = false;
            return;
        }
    }

    // load new step
    remaining_ms = pattern[step_idx].length * MS_PER_SIXTEENTH;
    strumState = pattern[step_idx].state;
    state_update();
    // // For use with Step struct
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
