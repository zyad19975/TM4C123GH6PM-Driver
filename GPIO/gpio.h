
#include "TM4C123GH6PM.h"

#ifndef __GPIO_H__
#define __GPIO_H__

#define OUTPUT 1
#define OUT 1
#define Out 1
#define out 1
#define Output 1
#define output 1

#define INPUT 0
#define Input 0
#define in 0
#define input 0
#define In 0
#define IN 0

#define HIGH 1
#define high 1
#define High 1

#define LOW 0
#define Low 0 
#define low 0

void DigitalMode (char port, short Port_num, bool direction);
void DigitalWrite (char port, short Port_num, bool a);
bool DigitalRead (char port, short Port_num);


#endif 