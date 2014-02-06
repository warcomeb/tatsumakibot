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
 * @file mainwindow.h
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief Main window class definitions.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "serialsetup.h"
#include "protocol.h"

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void setupSerialPort();
    void connectSerialPort();
    void disconnectSerialPort();

    void sendSerialData();
    void receiveSerialData();

private:
    Ui::MainWindow *ui;

    QSerialPort* m_serialPort;
    SerialSetup* m_serialSetup;
    SerialSetup::SerialSettings m_serialSettings;
    Protocol::BusStatus m_serialBusStatus;
    bool m_isSerialConnected;
    QByteArray m_serialReceiveBuffer;

    bool m_speedUp;

    Protocol::MessageParameters m_message;
    Protocol::ReplyMessageParameters m_replyMessage;

    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

    void initActionsConnections();

    void enableSendingButtons();
    void disableSendingButtons();
};

#endif // MAINWINDOW_H
