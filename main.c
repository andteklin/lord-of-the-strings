/* main.c  
    
*/
#include "pwm.h"
#include "MKL46Z4.h"
#include "strummer.h"

int main(void){
    SystemCoreClockUpdate();
    strummer_init();
    Buttons_Init();
    strummer_selectPattern(2); // classic pattern
    strummer_setBPM(120);
    strummer_enable(true);
    while (1) {
        
    }
    return 0;
}