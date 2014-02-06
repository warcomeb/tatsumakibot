/******************************************************************************
 * TatsumakiBot Desktop Control Interface
 * Copyright (C) 2014 Marco Giammarini
 *
 * Author(s):
 *  Marco Giammarini <m.giammarini@warcomeb.it>
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
 * @file protocol.cpp
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief Protocol class methods definitions.
 */

#include "protocol.h"

#include <QDebug>

#define PROTOCOL_ACK_LENGTH         6

const char Protocol::messageStart = '#';
const char Protocol::messageStop  = '!';

/**
 * @brief Protocol::composeMessage
 * @param parameters
 * @param string
 */
void Protocol::composeMessage(const Protocol::MessageParameters parameters,
                              QByteArray &string)
{
    // Clear all chars in a string
    string.clear();

    // Add start message char
    string.append(messageStart);

    // Add function ID
    string.append((QString("%1").arg(parameters.command,2,16,QChar('0'))).toUpper());

    // Add message type dependent part of string
    switch (parameters.command)
    {
    case RobotMove:
        string.append((QString("%1").arg((qint8)parameters.direction,2,16,QChar('0'))).toUpper());
        string.append((QString("%1").arg((qint8)parameters.speed,2,16,QChar('0'))).toUpper());
        break;
    default:
        Q_ASSERT(0);
        break;
    }

    // Compute checksum
    quint8 checksum = computeChecksum(string,1,string.size());
    string.append((QString("%1").arg(checksum,2,16,QChar('0'))).toUpper());

    // Add end message char
    string.append(messageStop);
}

/**
 * @brief Protocol::messageToString
 * @param message
 * @param string
 */
void Protocol::messageToString(const QByteArray &message, QString &string)
{
    string.clear();
    string.append(messageStart);

    // Delete start and end chars
    for (int i = 1; i < message.size()-1; ++i)
        string.append(message[i]);

    string.append(messageStop);
}

/**
 * @brief computeChecksum
 *
 * This method compute [LRC checksum](http://en.wikipedia.org/wiki/Longitudinal_redundancy_check)
 * of the data contained into @a message param, from @a start point and span to
 * @a stop point.
 *
 * @param message
 * @param start
 * @param stop
 * @return computed checksum
 */
quint8 Protocol::computeChecksum(const QByteArray &message, int start, int stop)
{
    quint8 lrc = 0;

    for (int i = start; i < stop; ++i) lrc += message.at(i);

    return ((quint8) -lrc);
}

bool Protocol::controlReceivedMessage(const Protocol::MessageParameters &parameters,
                                      const QByteArray &receivedMessage)
{
    qDebug() << "Serial receive message: control message";

    // Control checksum
    quint8 checksum = computeChecksum(receivedMessage,1,receivedMessage.length()-3);
    QString checksumString = (QString("%1").arg(checksum,2,16,QChar('0'))).toUpper();
    if (QString::compare(checksumString,QString(receivedMessage.right(3).left(2))) != 0)
    {
        qDebug() << "Serial receive message: checksum error compute (" <<
                    checksumString << ") received (" << QString(receivedMessage.right(3).left(2));
        return false;
    }
    qDebug() << "Serial receive message: checksum passed";


    ReplyMessageType receivedCommand = static_cast<ReplyMessageType>(
                QString(receivedMessage.left(3).right(2)).toUInt(0,16));

    // Manage message
    switch(parameters.command)
    {
    case Protocol::RobotMove:
        if (receivedCommand != Protocol::Acknowledge)
        {
            qDebug() << "Serial receive message: command reply error (" <<
                        QString(receivedMessage.left(3).right(2)) << ")";
            return false;
        }
        qDebug() << "Serial receive message: command reply passed!";

        if (receivedMessage.length() != PROTOCOL_ACK_LENGTH)
        {
            qDebug() << "Serial receive message: message length error!";
            return false;
        }
        qDebug() << "Serial receive message: message length passed!";

        break;
    default:
        qDebug() << "Serial receive message: not requested!!";
        return false;
    }

    qDebug() << "Serial receive message: control message <<OK>>";
    return true;
}

void Protocol::parseReceivedMessage(const QByteArray &receivedMessage,
                                    Protocol::ReplyMessageParameters &parameters)
{
    qDebug() << "Serial receive message: parse message";

    parameters.command = static_cast<ReplyMessageType>(
                QString(receivedMessage.left(3).right(2)).toUInt(0,16));
}
