/* main.c  
    
*/
#include "pwm.h"
#include "board.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "strummer.h"
// #include "strummer.h"
// #include "systick.h" // or wherever you implement millis

// int main(void) {
//     init_systick();     // Sets up global millis using SysTick
//     strummer_init();    // Initializes strummer (servo at rest)
//     strummer_enable(true); // Start strumming

//     while (1) {
//         strummer_update(); // non-blocking!
//         // ... other tasks
//     }
// }

int main(void){
    strummer_init();
    strummer_enable(true);
    while (1) {
        
    }
    return 0;
}
volatile static int i = 1; // counter in memory
