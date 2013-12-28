/******************************************************************************
 * Copyright (C) 2013-2014 Marco Giammarini
 * 
 * Author(s):
 *  Marco Giammarini <m.giammarini@warcomeb.it>
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
 * @file Project_Headers/pinout.c
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief ARM matrix pinout definitions. 
 */

#include "pinout.h"
#include "board.h"

void setPinout (void)
{
#if (PCB_VERSION == 0)

    /* Set mux for motor direction pins: */
    MOTOR1_IN1_PIN = PORT_PCR_MUX(MOTOR1_IN1_MUX);
    MOTOR1_IN2_PIN = PORT_PCR_MUX(MOTOR1_IN2_MUX);
    MOTOR2_IN1_PIN = PORT_PCR_MUX(MOTOR2_IN1_MUX);
    MOTOR2_IN2_PIN = PORT_PCR_MUX(MOTOR2_IN2_MUX);
    
    SYS_LED_PIN = PORT_PCR_MUX(SYS_LED_MUX);
    ERR_LED_PIN = PORT_PCR_MUX(ERR_LED_MUX);
    
#endif
}

void initGpio (void)
{    
#if (PCB_VERSION == 0)
    
    /* Initialization motor direction pins: */
    MOTOR1_IN1_PDDR |= GPIO_PDDR_PDD(MOTOR1_IN1_NUMBER);
    MOTOR1_IN2_PDDR |= GPIO_PDDR_PDD(MOTOR1_IN2_NUMBER);
    MOTOR1_DISABLE();
    
    MOTOR2_IN1_PDDR |= GPIO_PDDR_PDD(MOTOR2_IN1_NUMBER);
    MOTOR2_IN2_PDDR |= GPIO_PDDR_PDD(MOTOR2_IN2_NUMBER);
    MOTOR2_DISABLE();
    
    SYS_LED_PDDR |= GPIO_PDDR_PDD(SYS_LED_NUMBER);
    SYS_LED_OFF();
    ERR_LED_PDDR |= GPIO_PDDR_PDD(ERR_LED_NUMBER);
    ERR_LED_OFF();

#endif
}
