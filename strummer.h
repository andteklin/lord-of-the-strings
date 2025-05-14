/* strummer.h
    Functions to define strumming motions.
*/

#ifndef STRUMMER_H
#define STRUMMER_H

#include <stdint.h>
#include <stdbool.h>

void strummer_init(void);
void strummer_update(void);
void strummer_enable(bool on);
void PIT_Init();

typedef enum {
    DUR_16 = 1,    // one 16th
    DUR_8  = 2,    // two 16ths
    DUR_4  = 4     // four 16ths (quarter)
} NoteDiv;

/* Five possible states.
*/
typedef enum {
    STRUM_DOWN,
    STRUM_UP,
    MUTE_ON,
    MUTE_OFF,
    REST
} StrumState;

/* 
Step struct for defining a schedule based on timestamp
*/
typedef struct {
    uint16_t time_ms;
    StrumState state;
} Step;

/* PatternStep struct for defining a schedule based on note length */
typedef struct {
    StrumState state;
    NoteDiv    length;  // in units of 16th‑notes
} PatternStep;

#endif
