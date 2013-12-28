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
 * @file Sources/timer.c
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief Timer initialization for wait functions.
 */

#include "board.h"

#include "timer.h"

static uint32_t Timer_milliseconds;

void Timer_isr (void)
{
    Timer_milliseconds++;

    /* Clear ISR */
    (void) (FTM0_SC==0);
    FTM0_SC = FTM_SC_TOIE_MASK | FTM_SC_CLKS(1) | FTM_SC_PS(0x3) | 0;
}

void Timer_init (void)
{
    /* Configuring NVIC */
    Interrupt_enable(INTERRUPT_FTM0);

    /* Enable clock gate for FTM0 module */
    SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;

    /* 12500 pulse @ 80ns = 1ms */
    FTM0_MOD = 12499;
    /* Enable timer interrupt, system clock and prescaler set to divide x8 (12.5MHz) */
    FTM0_SC = FTM_SC_TOIE_MASK | FTM_SC_CLKS(1) | FTM_SC_PS(0x3) | 0;

    Timer_milliseconds = 0;
}

void Timer_delay (uint32_t msDelay)
{
    uint32_t currTicks = Timer_milliseconds;
    
    while ((Timer_milliseconds - currTicks) < msDelay);
}

uint32_t Timer_currentMillis (void)
{
    return Timer_milliseconds;
}
