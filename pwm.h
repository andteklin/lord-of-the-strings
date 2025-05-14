/* pwm.h
*/
#ifndef PWM_H_
#define PWM_H_
#include <stdint.h>
uint16_t angle_to_cnV(uint8_t angle);
void setupPWM(void);
void straight();
void turnCW90();
void turnCCW90();
#endif /* PWM_H_ */
