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
 * @file metadata.h
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief Metadata of the project.
 */

#ifndef METADATA_H
#define METADATA_H

#include <QSerialPort>

#define ORGANIZATION_NAME    "Marco Giammarini"
#define ORGANIZATION_DOMAIN  "warcomeb.it"

#define PROJECT_NAME         "TatsumakiBot Interface"
#define PROJECT_VERSION      "0.1.0"

#define RELEASE_DATE         "Xxxxxxx NN, 2014"

/* Serial communication parameters */
#define SERIAL_BAUDRATE      QSerialPort::Baud38400
#define SERIAL_DATABITS      QSerialPort::Data8
#define SERIAL_FLOWCONTROL   QSerialPort::NoFlowControl
#define SERIAL_PARITY        QSerialPort::NoParity
#define SERIAL_STOPBITS      QSerialPort::OneStop

#endif // METADATA_H
