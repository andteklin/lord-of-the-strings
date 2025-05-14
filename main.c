/* main.c  
    
*/
#include "pwm.h"
#include "MKL46Z4.h"
#include "strummer.h"

extern uint8_t curPID;
extern uint8_t BPM;
// void switchPattern(uint8_t id) {
//     if (id < NUM_PATTERNS) {
//         curPattern = patterns[id];
//         curLen     = patternLens[id];
//         strummer_enable(true, /*loop=*/true);
//     }
// }
int main(void){
    strummer_init();
    // Buttons_Init();
    curPID = 2;
    BPM=240;
    strummer_enable(true);
    while (1) {
        
    }
    return 0;
}