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
 * @file Project_Headers/board.h
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief Board utility defines. 
 */

#ifndef __BOARD_H
#define __BOARD_H

//#include "derivative.h"
#include "libohiboard.h" 

#define FW_MAJOR_VERSION         1
#define FW_MINOR_VERSION         0
/**
 *  Time stamp of current firmware version (-5GMT + DST).
 *  http://www.currenttimestamp.com/
 */
#define FW_TIME_VERSION          1378801518 /* TODO: Update */
/** 
 * Macro for board version.
 * 
 * 0: BreadBoard
 * 1: FRDM-KL25Z
 * 2: LOC028
 */
#define PCB_VERSION              0
#define PROJECT_NAME             "TatsumakiBot"
#define PROJECT_COPYRIGTH        "(C) 2013-2014 Marco Giammarini"

#if (PCB_VERSION == 0)

/* Motor control pins definitions */
#define MOTOR1_IN1_PIN           PORTC_PCR4
#define MOTOR1_IN1_NUMBER        GPIO_PIN(4)
#define MOTOR1_IN1_PDOR          GPIOC_PDOR
#define MOTOR1_IN1_PDDR          GPIOC_PDDR
#define MOTOR1_IN1_PCOR          GPIOC_PCOR
#define MOTOR1_IN1_PSOR          GPIOC_PSOR
#define MOTOR1_IN1_PTOR          GPIOC_PTOR
#define MOTOR1_IN1_MUX           1

#define MOTOR1_IN2_PIN           PORTC_PCR5
#define MOTOR1_IN2_NUMBER        GPIO_PIN(5)
#define MOTOR1_IN2_PDOR          GPIOC_PDOR
#define MOTOR1_IN2_PDDR          GPIOC_PDDR
#define MOTOR1_IN2_PCOR          GPIOC_PCOR
#define MOTOR1_IN2_PSOR          GPIOC_PSOR
#define MOTOR1_IN2_PTOR          GPIOC_PTOR
#define MOTOR1_IN2_MUX           1

#define MOTOR1_DISABLE() do{     \
    MOTOR1_IN1_PCOR |= GPIO_PCOR_PTCO(MOTOR1_IN1_NUMBER); \
    MOTOR1_IN2_PCOR |= GPIO_PCOR_PTCO(MOTOR1_IN2_NUMBER); \
    } while (0)

#define MOTOR2_IN1_PIN           PORTC_PCR6
#define MOTOR2_IN1_NUMBER        GPIO_PIN(6)
#define MOTOR2_IN1_PDOR          GPIOC_PDOR
#define MOTOR2_IN1_PDDR          GPIOC_PDDR
#define MOTOR2_IN1_PCOR          GPIOC_PCOR
#define MOTOR2_IN1_PSOR          GPIOC_PSOR
#define MOTOR2_IN1_PTOR          GPIOC_PTOR
#define MOTOR2_IN1_MUX           1

#define MOTOR2_IN2_PIN           PORTC_PCR7
#define MOTOR2_IN2_NUMBER        GPIO_PIN(7)
#define MOTOR2_IN2_PDOR          GPIOC_PDOR
#define MOTOR2_IN2_PDDR          GPIOC_PDDR
#define MOTOR2_IN2_PCOR          GPIOC_PCOR
#define MOTOR2_IN2_PSOR          GPIOC_PSOR
#define MOTOR2_IN2_PTOR          GPIOC_PTOR
#define MOTOR2_IN2_MUX           1

#define MOTOR2_DISABLE() do{     \
    MOTOR2_IN1_PCOR |= GPIO_PCOR_PTCO(MOTOR2_IN1_NUMBER); \
    MOTOR2_IN2_PCOR |= GPIO_PCOR_PTCO(MOTOR2_IN2_NUMBER); \
    } while (0)


/* OHIBoard LEDs */
#define SYS_LED_PIN              PORTD_PCR4
#define SYS_LED_NUMBER           GPIO_PIN(4)
#define SYS_LED_PDOR             GPIOD_PDOR
#define SYS_LED_PDDR             GPIOD_PDDR
#define SYS_LED_PCOR             GPIOD_PCOR
#define SYS_LED_PSOR             GPIOD_PSOR
#define SYS_LED_PTOR             GPIOD_PTOR
#define SYS_LED_MUX              1

#define SYS_LED_ON()             (SYS_LED_PCOR |= GPIO_PCOR_PTCO(SYS_LED_NUMBER))
#define SYS_LED_OFF()            (SYS_LED_PSOR |= GPIO_PSOR_PTSO(SYS_LED_NUMBER))
#define SYS_LED_TOGGLE()         (SYS_LED_PTOR |= GPIO_PTOR_PTTO(SYS_LED_NUMBER))

#define ERR_LED_PIN              PORTD_PCR7
#define ERR_LED_NUMBER           GPIO_PIN(7)
#define ERR_LED_PDOR             GPIOD_PDOR
#define ERR_LED_PDDR             GPIOD_PDDR
#define ERR_LED_PCOR             GPIOD_PCOR
#define ERR_LED_PSOR             GPIOD_PSOR
#define ERR_LED_PTOR             GPIOD_PTOR
#define ERR_LED_MUX              1

#define ERR_LED_ON()             (ERR_LED_PCOR |= GPIO_PCOR_PTCO(ERR_LED_NUMBER))
#define ERR_LED_OFF()            (ERR_LED_PSOR |= GPIO_PSOR_PTSO(ERR_LED_NUMBER))
#define ERR_LED_TOGGLE()         (ERR_LED_PTOR |= GPIO_PTOR_PTTO(ERR_LED_NUMBER))

/* Output Test pin (OHIBoard-IO58): */
#define TEST_PIN                 PORTD_PCR3
#define TEST_NUMBER              GPIO_PIN(3)
#define TEST_PDOR                GPIOD_PDOR
#define TEST_PDDR                GPIOD_PDDR
#define TEST_PCOR                GPIOD_PCOR
#define TEST_PSOR                GPIOD_PSOR
#define TEST_PTOR                GPIOD_PTOR
#define TEST_MUX                 1

#define TEST_CLEAR()             (TEST_PCOR |= GPIO_PCOR_PTCO(TEST_NUMBER))
#define TEST_SET()               (TEST_PSOR |= GPIO_PSOR_PTSO(TEST_NUMBER))
#define TEST_TOGGLE()            (TEST_PTOR |= GPIO_PTOR_PTTO(TEST_NUMBER))

#endif

#endif /* __BOARD_H */
