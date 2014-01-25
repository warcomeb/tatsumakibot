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
 * @file protocol.h
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief Protocol class definitions.
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "metadata.h"

#include <QByteArray>
#include <QString>

#include <stdint.h>

/**
 * @brief The Protocol class
 */
class Protocol
{

public:

    /**
     * @brief The MessageType enum
     */
    enum MessageType
    {
        RobotMove,
        None
    };

    /**
     * @brief The MessageParameters struct
     */
    struct MessageParameters
    {
        MessageType  command;
    };


//    static void clearParametersList(Parameters &parameters);

    static void composeMessage(const MessageParameters parameters,  QByteArray &string);
    static void messageToString(const QByteArray &message, QString &string);

//    static bool controlReceivedMessage(const Parameters &parameters,
//                                       const QByteArray &receivedMessage);

    static quint8 computeChecksum(const QByteArray &message, int start, int stop);
};

#endif // PROTOCOL_H
