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

#include "motor.h"

#define COMM_ACTIVE                     1
#define COMM_NO_ACTIVE                  0
#define COMM_BAUDRATE                   38400

#define COMM_RX_BUFFER_MASK             0xFF
#define COMM_TX_BUFFER_MASK             0xFF

#define COMM_START                      '#'
#define COMM_END                        '!'

/* Commands definition: */
/* Robot to Wifi */
/* TODO */
#define COMM_CMD_ACK                    0x01
#define COMM_CMD_ACK_LENGTH             6
#define COMM_CMD_NACK                   0x03

/* WiFi to Robot: */
/* TODO */
#define COMM_CMD_MOVEROBOT              0x10
#define COMM_CMD_MOVEROBOT_LENGTH       10

#define COMM_CMD_MAX_LENGTH             20

static union Comm_RxMessageType
{
    uint8_t buffer[COMM_RX_BUFFER_MASK + 1];
    
    struct 
    {
        uint8_t startChar;
        uint8_t command[2];
    } header;
    
    struct
    {
        uint8_t header[3];
        uint8_t direction[2];
        uint8_t speed[2];
        uint8_t footer[3];
    } robotMove;
    
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

void Comm_addReceivedChar (void)
{
    char c;

    Uart_getChar(Comm_device,&c);

    switch (c)
    {
    case COMM_START:
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
        Comm_rxMessage.buffer[Comm_rxBufferIndex] = c;
        Comm_rxBufferIndex++;
        Comm_rxBufferIndex &= COMM_RX_BUFFER_MASK;
        break;
    case COMM_END:
        /* If we have received the last char... */
        Comm_rxMessage.buffer[Comm_rxBufferIndex] = c;
        Board_taskStatus.flags.commandReady = 1;
        Comm_rxBufferIndex++;
        Comm_rxBufferIndex &= COMM_RX_BUFFER_MASK;
        break;
    default: /* Wrong char */
        Comm_rxBufferIndex = 0;
        /* TODO: Gestire errore!! */
        break;
    }

    /* TODO: Errore e resettare il comando!*/
    if (!Board_taskStatus.flags.commandReady && Comm_rxBufferIndex > COMM_CMD_MAX_LENGTH);
}

static uint8_t Comm_computeChecksum (const uint8_t* data, uint8_t start, uint8_t length)
{
    uint8_t lrc = 0;
    uint8_t i;

    for (i = start; i < length; ++i)
    {
        lrc += *data;
        data++;
    }

    return ((uint8_t) -lrc);
}

typedef enum _Comm_AcknowledgeType
{
    COMM_ACKNOWLEDGE_ACK,
    COMM_ACKNOWLEDGE_NACK,    
} Comm_AcknowledgeType;

/**
 * 
 */
static void Comm_composeAcknowledgeMessage (Comm_AcknowledgeType ackType)
{
    uint8_t checksum;
    
    Comm_txMessage.header.startChar = COMM_START;
    if (ackType == COMM_ACKNOWLEDGE_ACK)
        u8tx(Comm_txMessage.header.command,COMM_CMD_ACK);
    else
        u8tx(Comm_txMessage.header.command,COMM_CMD_NACK);
    checksum = Comm_computeChecksum(&Comm_txMessage.buffer[1],1,COMM_CMD_ACK_LENGTH-3);
    u8tx(Comm_txMessage.ack.checksum,checksum);
    Comm_txMessage.ack.endChar = COMM_END;
    Comm_txBufferIndex = COMM_CMD_ACK_LENGTH;
}

/**
 * 
 */
static void Comm_sendMessage (void)
{
    uint16_t i;
    for (i = 0; i < Comm_txBufferIndex; ++i)
    {
        Uart_putChar(Comm_device,Comm_txMessage.buffer[i]);
    }
}

/**
 * @brief Comm_parseCommand
 * 
 * @return Board_Errors
 */
Board_Errors Comm_parseCommand (void)
{
    static Board_Errors errors = ERRORS_NOERRORS;
    
    static uint8_t rxCommand = 0;
    
    static uint8_t computeChecksum = 0;
    static uint8_t rxChecksum = 0;

    static uint8_t direction = 0;
    static uint8_t speed = 0;
    
    static uint8_t messageLength = 0;
    
    /* Reset buffer index indicator */
    messageLength = Comm_rxBufferIndex;
    Comm_rxBufferIndex = 0;
    
    /* Control start and end chars of message */
    if ((Comm_rxMessage.header.startChar == COMM_START) && (Comm_rxMessage.buffer[messageLength-1] == COMM_END))
    {
        /* Control checksum */
        xtu8(&Comm_rxMessage.buffer[messageLength-3],&rxChecksum,2);
        computeChecksum = Comm_computeChecksum (&Comm_rxMessage.buffer[1],1,messageLength-3);
        if (computeChecksum != rxChecksum)
            return ERRORS_COMM_CHECKSUM; /* Checksum mismatch */
        
        /* Message parsing... */
        xtu8(Comm_rxMessage.header.command,&rxCommand,2);
        switch (rxCommand)
        {
        case COMM_CMD_MOVEROBOT:
            if (messageLength != COMM_CMD_MOVEROBOT_LENGTH) return ERRORS_COMM_WRONG_LENGHT;
            
            xtu8(Comm_rxMessage.robotMove.direction,&direction,2);
            xtu8(Comm_rxMessage.robotMove.speed,&speed,2);
            errors = Motor_move(direction,speed);
            if (errors == ERRORS_MOTOR_OK)
            {
                /* If the message does't have any problem, we reply with ACK */
                Comm_composeAcknowledgeMessage(COMM_ACKNOWLEDGE_ACK);
            }
            else
            {
                /* Otherwise, we reply with NACK */
                Comm_composeAcknowledgeMessage(COMM_ACKNOWLEDGE_NACK);                
            }
            Comm_sendMessage();
            break;
        default:
            return ERRORS_COMM_WRONG_COMMAND;
        }
        
        return ERRORS_COMM_OK;
    }
    else
    {
        return ERRORS_COMM_NOT_COMPLIANT;
    }
}
