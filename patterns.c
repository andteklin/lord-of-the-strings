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
