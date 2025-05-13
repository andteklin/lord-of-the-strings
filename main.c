/* main.c  
    
*/
#include "pwm.h"

int main(void){
    setup();
    turnCW90();
    while(1);
    return 0;
}
volatile static int i = 1; // counter in memory
