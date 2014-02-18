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


#define MOTOR_DEFAULT_SPEED           0.3 
/** Define the max speed of the motor based on PWM duty cycle */
#define MOTOR_MAX_SPEED               0.8 

//#define MOTOR_SPEED_DIVISION(speed)  (speed/100)

static float Motor_currentSpeed;

static Ftm_Config Motor_pwmConfig = 
{
    .mode              = FTM_MODE_PWM,

    .timerFrequency    = 50000,
    .initCounter       = 0,

    .pins              = {FTM_PINS_PTB0,FTM_PINS_PTB1,FTM_PINS_STOP},
    .duty              = {0.3 * 32768, 
                          0.3 * 32768},
    
    .configurationBits = FTM_CONFIG_PWM_EDGE_ALIGNED | FTM_CONFIG_PWM_POLARITY_LOW | 0,
};

/* Motor definitions */
static Motor_Config Motor_left = 
{
    .enable1PinPtr           = &MOTOR1_IN1_PIN,
    .enable1PinDirectionPtr  = &MOTOR1_IN1_PDDR,
    .enable1ClearPtr         = &MOTOR1_IN1_PCOR,
    .enable1SetPtr           = &MOTOR1_IN1_PSOR,
    .enable1PinNumber        = MOTOR1_IN1_NUMBER,
    .enable1PinMux           = MOTOR1_IN1_MUX,
    
    .enable2PinPtr           = &MOTOR1_IN2_PIN,
    .enable2PinDirectionPtr  = &MOTOR1_IN2_PDDR,
    .enable2ClearPtr         = &MOTOR1_IN2_PCOR,
    .enable2SetPtr           = &MOTOR1_IN2_PSOR,
    .enable2PinNumber        = MOTOR1_IN2_NUMBER,
    .enable2PinMux           = MOTOR1_IN2_MUX,
    
    .pwmChannelOutput        = FTM_CHANNELS_CH0
};

static Motor_Config Motor_right = 
{
    .enable1PinPtr           = &MOTOR2_IN1_PIN,
    .enable1PinDirectionPtr  = &MOTOR2_IN1_PDDR,
    .enable1ClearPtr         = &MOTOR2_IN1_PCOR,
    .enable1SetPtr           = &MOTOR2_IN1_PSOR,
    .enable1PinNumber        = MOTOR2_IN1_NUMBER,
    .enable1PinMux           = MOTOR2_IN1_MUX,
    
    .enable2PinPtr           = &MOTOR2_IN2_PIN,
    .enable2PinDirectionPtr  = &MOTOR2_IN2_PDDR,
    .enable2ClearPtr         = &MOTOR2_IN2_PCOR,
    .enable2SetPtr           = &MOTOR2_IN2_PSOR,
    .enable2PinNumber        = MOTOR2_IN2_NUMBER,
    .enable2PinMux           = MOTOR2_IN2_MUX,
    
    .pwmChannelOutput        = FTM_CHANNELS_CH1
};


void Motor_init (void)
{
    Ftm_init(FTM1,0,&Motor_pwmConfig);
    
    /* Set mux for motor direction pins and pull-down: */
    *Motor_left.enable1PinPtr = PORT_PCR_MUX(Motor_left.enable1PinMux) | PORT_PCR_PE_MASK;
    *Motor_left.enable2PinPtr = PORT_PCR_MUX(Motor_left.enable2PinMux) | PORT_PCR_PE_MASK;
    *Motor_right.enable1PinPtr = PORT_PCR_MUX(Motor_right.enable1PinMux) | PORT_PCR_PE_MASK;
    *Motor_right.enable2PinPtr = PORT_PCR_MUX(Motor_right.enable2PinMux) | PORT_PCR_PE_MASK;

    /* Initialization motor direction pins: */
    *Motor_left.enable1PinDirectionPtr |= GPIO_PDDR_PDD(Motor_left.enable1PinNumber);
    *Motor_left.enable2PinDirectionPtr |= GPIO_PDDR_PDD(Motor_left.enable2PinNumber);
    *Motor_right.enable1PinDirectionPtr |= GPIO_PDDR_PDD(Motor_right.enable1PinNumber);
    *Motor_right.enable2PinDirectionPtr |= GPIO_PDDR_PDD(Motor_right.enable2PinNumber);
    
    /* Stop the motors! */
    Motor_move(MOTOR_DIRECTION_STOP,MOTOR_DEFAULT_SPEED);
}

/**
 * 
 * @param direction 
 * @param speed
 */
Board_Errors Motor_move (Motor_Direction direction, uint8_t speed)
{
    static float computeSpeed = 0.0;
    
    computeSpeed = (float) speed / 100;
    
    if (computeSpeed > MOTOR_MAX_SPEED)
        return ERRORS_MOTOR_WRONG_SPEED;
    
    /* Setup directions */
    
    if (Motor_currentSpeed != computeSpeed)
    {
        Motor_currentSpeed = computeSpeed;
        Ftm_setPwm(FTM1,FTM_CHANNELS_CH0,computeSpeed * 32768);
        Ftm_setPwm(FTM1,FTM_CHANNELS_CH1,computeSpeed * 32768);
    }
    
    switch (direction)
    {
    case MOTOR_DIRECTION_UP:
        *Motor_left.enable1SetPtr = GPIO_PSOR_PTSO(Motor_left.enable1PinNumber);
        *Motor_left.enable2ClearPtr = GPIO_PCOR_PTCO(Motor_left.enable2PinNumber);
        *Motor_right.enable1SetPtr = GPIO_PSOR_PTSO(Motor_right.enable1PinNumber);
        *Motor_right.enable2ClearPtr = GPIO_PCOR_PTCO(Motor_right.enable2PinNumber);
        break;
    case MOTOR_DIRECTION_DOWN:
        *Motor_left.enable2SetPtr = GPIO_PSOR_PTSO(Motor_left.enable2PinNumber);
        *Motor_left.enable1ClearPtr = GPIO_PCOR_PTCO(Motor_left.enable1PinNumber);
        *Motor_right.enable2SetPtr = GPIO_PSOR_PTSO(Motor_right.enable2PinNumber);
        *Motor_right.enable1ClearPtr = GPIO_PCOR_PTCO(Motor_right.enable1PinNumber);
        break;
    case MOTOR_DIRECTION_RIGHT:
        *Motor_left.enable1SetPtr = GPIO_PSOR_PTSO(Motor_left.enable1PinNumber);
        *Motor_left.enable2ClearPtr = GPIO_PCOR_PTCO(Motor_left.enable2PinNumber);
        *Motor_right.enable2SetPtr = GPIO_PSOR_PTSO(Motor_right.enable2PinNumber);
        *Motor_right.enable1ClearPtr = GPIO_PCOR_PTCO(Motor_right.enable1PinNumber);
        break;
    case MOTOR_DIRECTION_LEFT:
        *Motor_left.enable2SetPtr = GPIO_PSOR_PTSO(Motor_left.enable2PinNumber);
        *Motor_left.enable1ClearPtr = GPIO_PCOR_PTCO(Motor_left.enable1PinNumber);
        *Motor_right.enable1SetPtr = GPIO_PSOR_PTSO(Motor_right.enable1PinNumber);
        *Motor_right.enable2ClearPtr = GPIO_PCOR_PTCO(Motor_right.enable2PinNumber);
        break;
    case MOTOR_DIRECTION_STOP:
        *Motor_left.enable1ClearPtr = GPIO_PCOR_PTCO(Motor_left.enable1PinNumber);
        *Motor_left.enable2ClearPtr = GPIO_PCOR_PTCO(Motor_left.enable2PinNumber);
        *Motor_right.enable1ClearPtr = GPIO_PCOR_PTCO(Motor_right.enable1PinNumber);
        *Motor_right.enable2ClearPtr = GPIO_PCOR_PTCO(Motor_right.enable2PinNumber);
        break;
    case MOTOR_DIRECTION_NONE:
        return ERRORS_MOTOR_WRONG_DIRECTION;
    }
    
    return ERRORS_MOTOR_OK;
}
