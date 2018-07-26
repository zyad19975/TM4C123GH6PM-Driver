#include "TM4C123GH6PM.h"
#include "gpio.h"



void DigitalMode (char port, short Port_num, bool direction){

    switch port:
        //port A configurations
        case 'A'

            //Clock
            SYSCTL->RCGCGPIO  |= 0x01;
            GPIOA->DEN |= (1<<Port_num);


            if (direction)
                GPIOA->DIR |= (1<<Port_num);
            else{
                GPIOA->DIR &= ~(1<<Port_num);
                GPIOA->PDR |=  (1<<Port_num);
            }
            break;
        //port B configurations
        case 'B'

            //Clock
            SYSCTL->RCGCGPIO  |= 0x02;
            GPIOB->DEN |= (1<<Port_num);


            if (direction)
                GPIOB->DIR |= (1<<Port_num);
            else{
                GPIOB->DIR &= ~(1<<Port_num);
                GPIOB->PDR |=  (1<<Port_num);
            }
            break;
        //port C configurations        
        case 'C'

            //clock
            SYSCTL->RCGCGPIO  |= 0x04;
            GPIOC->DEN |= (1<<Port_num);


            if (direction)
                GPIOC->DIR |= (1<<Port_num);
            else{
                GPIOC->DIR &= ~(1<<Port_num);
                GPIOC->PDR |=  (1<<Port_num);
            }
            break;
        //port D configurations
        case 'D'

            //clock
            SYSCTL->RCGCGPIO  |= 0x08;
            GPIOD->DEN |= (1<<Port_num);


            if (direction)
                GPIOD->DIR |= (1<<Port_num);
            else{
                GPIOD->DIR &= ~(1<<Port_num);
                GPIOD->PDR |=  (1<<Port_num);
            }
            break;
        //port E configurations
        case 'E'

            //clock
            SYSCTL->RCGCGPIO  |= 0x10;
            GPIOE->DEN |= (1<<Port_num);


            if (direction)
                GPIOE->DIR |= (1<<Port_num);
            else{
                GPIOE->DIR &= ~(1<<Port_num);
                GPIOE->PDR |=  (1<<Port_num);
            }
            break;
        //port F configurations
        case 'F'

            //clock
            SYSCTL->RCGCGPIO  |= 0x20;
            GPIOF->DEN |= (1<<Port_num);

            if (direction)
                GPIOF->DIR |= (1<<Port_num);
            else{
                GPIOF->DIR &= ~(1<<Port_num);
                GPIOF->PDR |=  (1<<Port_num);
            }
            break;


}



void DigitalWrite (char port, short Port_num, bool a){

switch port:
        case 'A'

            if (a)
                GPIOA->DATA |= (1<<Port_num);
            else
                GPIOA->DATA &= ~(1<<Port_num);

            break;
        case 'B'

          if (a)
                GPIOB->DATA |= (1<<Port_num);
            else
                GPIOB->DATA &= ~(1<<Port_num);


            break;
        case 'C'

            if (a)
                GPIOC->DATA |= (1<<Port_num);
            else
                GPIOC->DATA &= ~(1<<Port_num);


            break;
        case 'D'

     
            if (a)
                GPIOD->DATA |= (1<<Port_num);
            else
                GPIOD->DATA &= ~(1<<Port_num);


            break;
        case 'E'


            if (a)
                GPIOE->DATA |= (1<<Port_num);
            else
                GPIOE->DATA &= ~(1<<Port_num);


            break;
        case 'F'

            if (a)
                GPIOF->DATA |= (1<<Port_num);
            else
                GPIOF->DATA &= ~(1<<Port_num);

            break;


}



bool DigitalRead (char port, short Port_num){


short x;
bool y;


switch port:
        case 'A'


        x = GPIOA->DATA
        y = x & (1<<Port_num);
        return y;

            break;
        case 'B'


        x = GPIOB->DATA
        y = x & (1<<Port_num);
        return y;


            break;
        case 'C'

        x = GPIOC->DATA
        y = x & (1<<Port_num);
        return y;


            break;
        case 'D'

        x = GPIOD->DATA
        y = x & (1<<Port_num);
        return y;


            break;
        case 'E'

        x = GPIOE->DATA
        y = x & (1<<Port_num);
        return y;

            break;
        case 'F'

        x = GPIOF->DATA
        y = x & (1<<Port_num);
        return y;

            break;


}
