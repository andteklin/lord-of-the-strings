/* main.c  
    
*/
#include "pwm.h"
#include "MKL46Z4.h"
#include "strummer.h"

int main(void){
    strummer_init();
    Buttons_Init();
    strummer_enable(true, true);
    while (1) {
        
    }
    return 0;
}