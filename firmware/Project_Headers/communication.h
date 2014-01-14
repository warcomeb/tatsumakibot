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
 * @file Project_Headers/communication.h
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief Communication function statement for programming protocol by USB/UART.
 */

#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include "libohiboard.h"
#include "board.h"

/* Enable functions for PC device */
Board_Errors Comm_init (Uart_DeviceHandle device);
Board_Errors Comm_enable (void);
Board_Errors Comm_disable (void);

void Comm_addReceivedChar (void);

Board_Errors Comm_parseCommand (void);

#endif /* __COMMUNICATION_H */
