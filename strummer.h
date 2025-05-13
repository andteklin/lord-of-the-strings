/* strummer.h
    Functions to define strumming motions.
*/

#ifndef STRUMMER_H
#define STRUMMER_H

#include <stdint.h>
#include <stdbool.h>

void strummer_init(void);
void strummer_update(void);
void strummer_enable(bool on);

#endif
