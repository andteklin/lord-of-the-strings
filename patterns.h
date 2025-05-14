/* patterns.h to define strumming patterns */
#ifndef PATTERNS_H
#define PATTERNS_H

#include "strummer.h"   // for PatternStep & StrumState

// Enumerate your patterns
typedef enum {
    PATTERN_BASIC,
    PATTERN_FUNKY,
    NUM_PATTERNS
} PatternId;

// Expose accessors for any pattern
const PatternStep*  Patterns_Get(PatternId id);
uint8_t             Patterns_Len(PatternId id);

#endif // PATTERNS_H
