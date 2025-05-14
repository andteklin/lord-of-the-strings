/* buttons.h to define buttons */
#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdbool.h>
#include <stdint.h>

#define BTN_TEMPO_UP     0
#define BTN_TEMPO_DOWN   1
#define BTN_MUTE_TOGGLE  2
#define BTN_STYLE_CYCLE  3
#define NUM_BUTTONS      4

void Buttons_Init(void);

#endif // BUTTONS_H
