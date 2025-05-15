/* strummer.h
    Functions to define strumming motions.
*/

#ifndef STRUMMER_H
#define STRUMMER_H

#include <stdint.h>
#include <stdbool.h>

/* Three possible note divisions for strumming/muting */
typedef enum {
    DUR_16 = 1,    // one 16th
    DUR_8  = 2,    // two 16ths
    DUR_D8 = 3,    // three 16ths
    DUR_4  = 4     // four 16ths (quarter)
} NoteDiv;

/* Three possible states for muting */
typedef enum {
    MUTE_NOCHANGE = 0,
    MUTE_ON,
    MUTE_OFF
} MuteState;

/* Three possible states for strumming */
typedef enum {
    STRUM_DOWN,
    STRUM_UP,
    REST
} StrumState;

/* Step struct for defining a schedule based on timestamp */
typedef struct {
    uint16_t time_ms;
    StrumState state;
} TimeStep;

/* StrumStep struct for defining a schedule based on note length */
typedef struct {
    StrumState state;
    NoteDiv    length;  // in units of 16th‑notes
} StrumStep;

/* ComboStep struct for defining a strumming/muting schedule baesd on note length */
typedef struct{
    StrumState strum;
    NoteDiv length; // length of note
    MuteState mute;
} ComboStep;

void strummer_init(void);
void strummer_setBPM(uint16_t bpm);
void strummer_update(void);
void strummer_enable(bool loop);
void strum_update(StrumState s);
void mute_update(MuteState m);
void strummer_toggleMute(void);
void strummer_stop(void);
void PIT_Init();
/**  
 * Select which pattern (0..NUM_PATTERNS‑1) to play on the next enable.  
 * Does nothing if pid >= NUM_PATTERNS.  
 */
void strummer_selectPattern(uint8_t pid);

#endif
