/* patterns.c to implement strumming patterns
To add a new pattern,
1. Define your static const ComboStep myNewPattern[] = { … };
2. Add a single line to the end of the patterns[] initializer 
    { .data = myNewPattern, .len = sizeof(myNewPattern)/sizeof(myNewPattern[0]) }
*/
#include "patterns.h"

// —— Pattern 0: simple down–up–down–up, rest ——  
static const ComboStep basic[] = {
    { STRUM_DOWN, DUR_16 },
    { STRUM_UP,   DUR_16 },
    { STRUM_DOWN, DUR_16 },
    { STRUM_UP,   DUR_16 },
    { REST,       DUR_8  },
};

// —— Pattern 1: a different groove ——  
static const ComboStep funky[] = {
    { STRUM_DOWN, DUR_8  },
    { REST,       DUR_16 },
    { STRUM_UP,   DUR_8  },
    { REST,       DUR_16 },
    { MUTE_ON,    DUR_4  },
};

// Pattern 2: The Classic Guitar Strumming Pattern
static const ComboStep classic[] = {
    { STRUM_DOWN, DUR_4 },
    { STRUM_UP,   DUR_8 },
    { STRUM_DOWN, DUR_8 },
    { REST,       DUR_8 },
    { STRUM_UP,   DUR_8 },
    { STRUM_DOWN, DUR_8 },
    { STRUM_UP,   DUR_8 },
};

// Pattern 3: a fun muted rhythm
static const ComboStep fun[] = {
    { STRUM_DOWN, DUR_D8, MUTE_ON },
    { STRUM_UP,   DUR_D8 },
    { STRUM_DOWN, DUR_D8 },
    { STRUM_UP,   DUR_D8 },
    { STRUM_DOWN, DUR_16 },
    { STRUM_UP,   DUR_16 },
    { STRUM_DOWN, DUR_16 },
    { STRUM_UP,   DUR_16 },
};

// /* :| _ ^ _ ^ _ ^ _ ^ _^ :| */
// TimeStep schedule[NUM_STEPS] = {
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

// Define pattern structs here
const Pattern patterns[] = {
    {basic,   sizeof(basic)   / sizeof(basic[0])   },
    {funky,   sizeof(funky)   / sizeof(funky[0])   },
    {classic, sizeof(classic) / sizeof(classic[0]) },
    {fun, sizeof(fun) / sizeof(fun[0]) }
};

const uint8_t NUM_PATTERNS = sizeof(patterns) / sizeof(patterns[0]);

const ComboStep* Patterns_GetData(uint8_t id) {
    return patterns[id].data;
}

uint8_t Patterns_GetLength(uint8_t id) {
    return patterns[id].len;
}
