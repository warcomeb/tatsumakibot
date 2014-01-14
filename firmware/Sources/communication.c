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
 * @file Sources/communication.c
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief Communication function definitions for programming protocol by USB/UART.
 */

#include "communication.h"
#include "board.h"

#define COMM_ACTIVE                     1
#define COMM_NO_ACTIVE                  0
#define COMM_BAUDRATE                   57600

#define COMM_RX_BUFFER_MASK             0xFF
#define COMM_TX_BUFFER_MASK             0xFF

#define COMM_START                      '#'
#define COMM_END                        '!'

/* Commands definition: */
/* Robot to Wifi */
/* TODO */
#define COMM_CMD_ACK                    0x01

/* WiFi to Robot: */
/* TODO */
#define COMM_CMD_MOVEMOTOR              0x10
#define COMM_CMD_MOVEMOTOR_LENGTH       8


static union Comm_RxMessageType
{
    uint8_t buffer[COMM_RX_BUFFER_MASK + 1];
    
    struct {
        uint8_t startChar;
        uint8_t command[2];
    } header;
    
} Comm_rxMessage;

static union Comm_TxMessageType
{
    uint8_t buffer[COMM_TX_BUFFER_MASK + 1];
    
    struct 
    {
        uint8_t startChar;
        uint8_t command[2];
    } header;

    struct 
    {
        uint8_t header[3];
        uint8_t checksum[2];
        uint8_t endChar;
    } ack;

} Comm_txMessage;

static uint8_t Comm_rxBufferIndex;
static uint8_t Comm_txBufferIndex;

static Uart_DeviceHandle Comm_device;
static uint8_t Comm_active = COMM_NO_ACTIVE;

Board_Errors Comm_init (Uart_DeviceHandle device)
{
    if (Comm_active == COMM_NO_ACTIVE)
    {
        Comm_device = device;
        Uart_setBaudRate(Comm_device,COMM_BAUDRATE);
        Uart_init(Comm_device);
        return ERRORS_COMM_OK;
    }
    else
    {
        return ERRORS_COMM_JUST_ACTIVE;        
    }
}

Board_Errors Comm_enable (void)
{
    if (Comm_active == COMM_NO_ACTIVE)
    {
        Uart_enable(Comm_device);
        Comm_active = COMM_ACTIVE;
        return ERRORS_COMM_OK;
    }
    else
    {
        return ERRORS_COMM_JUST_ACTIVE;        
    }
}

Board_Errors Comm_disable (void)
{
    if (Comm_active == COMM_ACTIVE)
    {
        Uart_disable(Comm_device);
        Comm_active = COMM_NO_ACTIVE;
        return ERRORS_COMM_OK;
    }
    else
    {
        return ERRORS_COMM_NO_ACTIVE;        
    }
}
