/* main.c  
    
*/
#include "pwm.h"
#include "board.h"
#include "clock_config.h"
#include "MKL46Z4.h"

int main(void){
    BOARD_InitBootClocks();
    BOARD_BootClockRUN(); // Configure FLL for 48MHz
    SystemCoreClockUpdate();
    setup();
    // SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;          // Enable Port B clock
    // PORTB->PCR[18] = (PORTB->PCR[18] & ~PORT_PCR_MUX_MASK)
    //            | PORT_PCR_MUX(1);           // ALT1 = GPIO
    // GPIOB->PDDR |= (1 << 18);                    // PTB18 = output
    // turnCW90();
    while (1) {
        straight();
        turnCW90();
        straight();
        turnCW90();
    }
    return 0;
}
volatile static int i = 1; // counter in memory
