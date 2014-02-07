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
 * @file Sources/motor.c
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief Motor functions implementation.
 */

#include "motor.h"

#define MOTOR_MAX_SPEED          80 /**< Define the max speed of the motor based on PWM duty cycle */

static Ftm_Config Motor_pwmConfig = {
    .mode              = FTM_MODE_PWM,

    .timerFrequency    = 50000,
    .initCounter       = 0,

    .pins              = {FTM_PINS_PTB0,FTM_PINS_PTB1,FTM_PINS_STOP},
    .duty              = {0.5 * 32768, 0.5 * 32768},
    
    .configurationBits = FTM_CONFIG_PWM_EDGE_ALIGNED | FTM_CONFIG_PWM_POLARITY_LOW | 0,
};

void Motor_init (void)
{
    Ftm_init(FTM1,0,&Motor_pwmConfig);
}

/**
 * 
 * @param direction 
 * @param speed
 */
Board_Errors Motor_move (Motor_Direction direction, uint8_t speed)
{
    if (speed > MOTOR_MAX_SPEED)
        return ERRORS_MOTOR_WRONG_SPEED;
}
