/* patterns.h to define strumming patterns */
#ifndef PATTERNS_H
#define PATTERNS_H


#include <stdint.h>
#include "strummer.h"   // for StrumStep & StrumState

typedef struct {
    const ComboStep *data;
    uint8_t len;
} Pattern;

extern const Pattern patterns[];
extern const uint8_t NUM_PATTERNS;

const ComboStep* Patterns_GetData   (uint8_t id);
uint8_t          Patterns_GetLength (uint8_t id);

#endif // PATTERNS_H
