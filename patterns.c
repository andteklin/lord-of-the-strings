/* patterns.c to implement strumming patterns */
#include "patterns.h"

// —— Pattern 0: simple down–up–down–up, rest ——  
static const PatternStep basic[] = {
    { STRUM_DOWN, DUR_16 },
    { STRUM_UP,   DUR_16 },
    { STRUM_DOWN, DUR_16 },
    { STRUM_UP,   DUR_16 },
    { REST,       DUR_8  },
};

// —— Pattern 1: a different groove ——  
static const PatternStep funky[] = {
    { STRUM_DOWN, DUR_8  },
    { REST,       DUR_16 },
    { STRUM_UP,   DUR_8  },
    { REST,       DUR_16 },
    { MUTE_ON,    DUR_4  },
};

// The Classic Guitar Strumming Pattern
static const PatternStep classic[] = {
    { STRUM_DOWN, DUR_4 },
    { STRUM_UP,   DUR_8 },
    { STRUM_DOWN, DUR_8 },
    { REST,       DUR_8  },
    { STRUM_UP,   DUR_8 },
    { STRUM_DOWN, DUR_8 },
    { STRUM_UP,   DUR_8 },
};

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

#define PATTERN_LEN  (sizeof pattern / sizeof pattern[0])
// Arrays of pointers & lengths
static const PatternStep*  allPatterns[NUM_PATTERNS] = {
    basic,
    funky
};
static const uint8_t       allLengths[NUM_PATTERNS] = {
    sizeof(basic) / sizeof(basic[0]),
    sizeof(funky) / sizeof(funky[0])
};

const PatternStep* Patterns_Get(PatternId id) {
    return allPatterns[id];
}

uint8_t Patterns_Len(PatternId id) {
    return allLengths[id];
}
