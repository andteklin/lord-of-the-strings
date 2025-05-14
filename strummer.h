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
void PIT_Init(uint32_t tick_ms);

/* Four possible states.
*/
typedef enum {
    STRUM_DOWN,
    STRUM_UP,
    MUTE_ON,
    MUTE_OFF
} StrumState;

/* 
Step struct for defining a schedule
*/
typedef struct {
    uint16_t time_ms;
    StrumState state;
} Step;

#endif
