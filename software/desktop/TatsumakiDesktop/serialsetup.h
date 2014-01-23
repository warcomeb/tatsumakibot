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
 * @file serialsetup.h
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief Serial setup dialog class definitions.
 */

#ifndef SERIALSETUP_H
#define SERIALSETUP_H

#include <QDialog>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class SerialSetup;
}

class SerialSetup : public QDialog
{
    Q_OBJECT

public:
    explicit SerialSetup(QWidget *parent = 0);
    ~SerialSetup();

    void setFixedParameters(enum QSerialPort::BaudRate rate,
                            enum QSerialPort::DataBits dataBits,
                            enum QSerialPort::FlowControl flowControl,
                            enum QSerialPort::Parity parity,
                            enum QSerialPort::StopBits stopBits);
    void clearFixedParameters();

private slots:
    void showPortInfos(int index);

private:
    Ui::SerialSetup *ui;

    bool m_isFixedParameter;

    void fillPortInfos();
    void fillPortParameters();
};

#endif // SERIALSETUP_H
