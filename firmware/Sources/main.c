/******************************************************************************
 * Copyright (C) 2013-2014 Marco Giammarini
 * 
 * Author(s):
 *	Marco Giammarini <m.giammarini@warcomeb.it>
 *	
 * Project: TatsumakiBot
 * Package: -
 * Version: 0.0
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>
 ******************************************************************************/

/**
 * @file Sources/main.c
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief Main file of the application. 
 */

/**
 * @mainpage TatsumakiBot - OHIBoard based mini-sumo robot
 * 
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 */

/* OHIBoard Library (LGPL) */
#include "libohiboard.h"

#include "board.h"
#include "pinout.h"
#include "timer.h"

#include "motor.h"

int main(void)
{
int counter = 0;

    /* Enable clock gate for ports to enable pin routing */
    SIM_SCGC5 |= (
            SIM_SCGC5_PORTA_MASK | 
            SIM_SCGC5_PORTB_MASK | 
            SIM_SCGC5_PORTC_MASK | 
            SIM_SCGC5_PORTD_MASK | 
            SIM_SCGC5_PORTE_MASK);
    
    /* Clock initialization */
    System_initClock();
    
    setPinout();
    initGpio();

    /* Setup base timer @ 1ms. */
    Timer_init();

    /* Hello by LEDs */
    SYS_LED_TOGGLE();
    Timer_delay(500);
    ERR_LED_TOGGLE();
    Timer_delay(500);
    SYS_LED_TOGGLE();
    Timer_delay(500);
    ERR_LED_TOGGLE();
    
    Motor_init();
    
for(;;) {	   
counter++;
}

return 0;
}
