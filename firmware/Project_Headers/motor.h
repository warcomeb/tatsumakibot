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
 * @file Project_Headers/motor.h
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief Motor functions declaration. 
 */

#ifndef __MOTOR_H
#define __MOTOR_H

#include "board.h"

typedef enum _Motor_Direction
{
    MOTOR_DIRECTION_STOP,
    MOTOR_DIRECTION_UP,
    MOTOR_DIRECTION_DOWN,
    MOTOR_DIRECTION_RIGHT,
    MOTOR_DIRECTION_LEFT,
    
    MOTOR_DIRECTION_NONE = 0xFF
} Motor_Direction;

typedef struct Motor_Config
{
    volatile uint32_t* enable1PinPtr;
    volatile uint32_t* enable1PinDirectionPtr;
    volatile uint32_t* enable1ClearPtr;
    volatile uint32_t* enable1SetPtr;
    uint32_t enable1PinNumber;
    uint8_t enable1PinMux;
    
    volatile uint32_t* enable2PinPtr;
    volatile uint32_t* enable2PinDirectionPtr;
    volatile uint32_t* enable2ClearPtr;
    volatile uint32_t* enable2SetPtr;
    uint32_t enable2PinNumber;
    uint8_t enable2PinMux;

    Ftm_Channels pwmChannelOutput;
} Motor_Config;

void Motor_init (void);

Board_Errors Motor_move (Motor_Direction direction, uint8_t speed);

#endif /* __MOTOR_H */
