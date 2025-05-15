/* strummer.c
    Implementations of functions in strummer.h
*/
#include "strummer.h"
#include "board.h"
#include "pwm.h"
#include "patterns.h"
#include "MKL46Z4.h"

// Pattern definitions
volatile uint8_t curPID;
static const ComboStep* curPattern;
static uint8_t curLen; // length of current pattern

// current strumming state
static volatile StrumState strumState;
// current muting state
static volatile MuteState muteState;

static uint32_t remaining_ms; // to track current time remaining
static uint8_t step_idx = 0; // to track which step in pattern we are in

static bool strumming_enabled = 0;
static bool loop = 0; // by default no looping of pattern

// Tempo and base durations
#define TICK_MS 10                                  // interrupt intervals for PIT
volatile uint16_t BPM;                      // beats per minute
static uint32_t ms_per_sixteenth;

// Servo Angle definitions
#define SU 0 // strumming up angle
#define SD 180 // strumming down angle
#define MU 29 // angle for mute bar up
#define MD 140 // angle for mute bar down

void strummer_init(void) {
    BOARD_InitBootClocks();
    BOARD_BootClockRUN(); // Configure FLL for 48MHz
    SystemCoreClockUpdate();
    setupPWM();
    PIT_Init(); // initialize PIT timer 10ms
    // Assumes TPM2 and servo channel already configured
    TPM2->CONTROLS[0].CnV = angle_to_cnV(SU); // Strummer up position
    TPM2->CONTROLS[1].CnV = angle_to_cnV(MU); // Mute Up position
}

/* Set the bpm of the strummer */
void strummer_setBPM(uint16_t bpm) {
    BPM = bpm;
    // 60000ms per minute, divided by bpm → ms per quarter note,
    // then /4 → ms per sixteenth.
    ms_per_sixteenth = (60000u / BPM) / 4;
}

/* Select pattern */
void strummer_selectPattern(uint8_t pid, bool doLoop) {
  if (pid >= NUM_PATTERNS) return;    // out of range guard

    // 1) stop current schedule & PIT interrupts
    PIT->CHANNEL[0].TCTRL &= ~(PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK);
    strumming_enabled = false;

    // 2) update the global PID and grab the new pattern
    curPID      = pid;
    curPattern  = Patterns_GetData(curPID);
    curLen      = Patterns_GetLength(curPID);

    // 3) reset step index and countdown
    step_idx     = 0;
    remaining_ms = curPattern[0].length * ms_per_sixteenth;
    strumState   = curPattern[0].strum;
    muteState    = curPattern[0].mute;

    // 4) apply the very first step immediately
    strum_update(strumState);
    mute_update(muteState);

    // 5) store whether we should loop
    loop = doLoop;

    // 6) clear any pending PIT flag, then re‑enable PIT
    PIT->CHANNEL[0].TFLG  = PIT_TFLG_TIF_MASK;
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;

    // 7) finally turn on scheduling
    strumming_enabled = true;
}

/* update the state of the strumming servo */
void strum_update(StrumState s) {
    switch (s) {
        case STRUM_DOWN:
            TPM2->CONTROLS[0].CnV = angle_to_cnV(SD);
            break;
        case STRUM_UP:
            TPM2->CONTROLS[0].CnV = angle_to_cnV(SU);
            break;
        case REST:
            break;
    }
}

void mute_update(MuteState m) {
    switch (m) {       
        case MUTE_ON:
            TPM2->CONTROLS[1].CnV = angle_to_cnV(MD);
            break;
        case MUTE_OFF:
            TPM2->CONTROLS[1].CnV = angle_to_cnV(MU);
            break;
        case MUTE_NOCHANGE:
            break;
    }
}

/* toggle muting */
void strummer_toggleMute(void) {
    // flip the logical state
    if (muteState == MUTE_ON) {
        muteState = MUTE_OFF;
    } else {
        muteState = MUTE_ON;
    }
    // apply it to the hardware
    mute_update(muteState);
}

/* Start or stop the pattern/schedule */
void strummer_enable() {
    // 1) Disable the PIT tick while we re‑init state
    PIT->CHANNEL[0].TCTRL &= ~(PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK);

    strumming_enabled = true;
    // set current pattern pointer
    curPattern = Patterns_GetData(curPID);
    curLen = Patterns_GetLength(curPID);

    step_idx = 0;
    remaining_ms = curPattern[0].length * ms_per_sixteenth;
    strumState = curPattern[0].strum;
    muteState = curPattern[0].mute;
    strum_update(strumState);
    mute_update(muteState);
    // 6) Clear any pending PIT flag
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;

    // 7) Re‑enable the PIT interrupt and timer
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;
}

/* called by PIT Interrupt Service Routine */
void strummer_update(void) {
    if (!strumming_enabled) return;
    // For use with StrumStep
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
    ComboStep const *s = &curPattern[step_idx];
    remaining_ms = s->length * ms_per_sixteenth;
    strumState = s->strum;
    muteState = s->mute;
    strum_update(strumState);
    mute_update(muteState);
}

void strummer_stop(void) {
    strumming_enabled = false;
    PIT->CHANNEL[0].TCTRL &= ~(PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK);

    remaining_ms = 0;
    loop = false;
    strum_update(REST);
    mute_update(MUTE_OFF);
    TPM2->CONTROLS[0].CnV = angle_to_cnV(SU); // Strummer up position
    TPM2->CONTROLS[1].CnV = angle_to_cnV(MU); // Mute Up position
}

/* Initialize PIT timer */
void PIT_Init() {
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; // Enable clock to PIT module
    PIT->MCR = 0; //Make sure the peripheral is enabled, should be on by default. 
    PIT->CHANNEL[0].LDVAL = ((SystemCoreClock/2)/1000 * TICK_MS) - 1; // Set timer period, about 1/10 sec
    PIT->CHANNEL[0].TFLG = 1; // clear previous interrupts
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; // enable countdown (Timer Interrupt Enable, Timer enable)
    NVIC_EnableIRQ(PIT_IRQn); // writes to ISER and ICER registers
}

/* interrupt every TICK_MS ms */
void PIT_IRQHandler(void) {
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
    strummer_update();
}
