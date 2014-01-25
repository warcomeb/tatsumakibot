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
 * @file mainwindow.cpp
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief Main window class methods implementations.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "metadata.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_serialPort(0)
{
    ui->setupUi(this);
    setWindowTitle(QString(PROJECT_NAME) + " v." + QString(PROJECT_VERSION));

    // Disable sending buttons
    ui->serialConnect->setEnabled(false);
    disableSendingButtons();

    m_serialSetup = new SerialSetup;
    // Send fix data to serial interface!
    m_serialSetup->setFixedParameters(SERIAL_BAUDRATE,
                                      SERIAL_DATABITS,
                                      SERIAL_FLOWCONTROL,
                                      SERIAL_PARITY,
                                      SERIAL_STOPBITS);

    // Connect signals to slots
    initActionsConnections();

    ui->serialStatus->setText(tr("No port connected!"));
}

MainWindow::~MainWindow()
{
    delete m_serialSetup;

    if (m_serialPort)
    {
        // Disable commuication port
        m_serialPort->close();
        delete m_serialPort;
    }

    delete ui;
}

/**
 * @brief MainWindow::setupSerialPort
 *
 * This method show setup dialog for communication port.
 * If connection was just established show a message error.
 */
void
MainWindow::setupSerialPort ()
{
    if (!m_serialPort)
    {
        m_serialSetup->fillPortInfos();
        m_serialSetup->exec();
        m_serialSettings = m_serialSetup->getSettings();

        if (m_serialSettings.name.isEmpty())
        {
            QMessageBox::critical(this, tr("Error"),
                tr("No port selected!"));
        }
        else
        {
            // Activate connect button and signal
            ui->serialConnect->setText("Connect");
            ui->serialConnect->setEnabled(true);
            connect(ui->serialConnect, SIGNAL(clicked()), this, SLOT(connectSerialPort()));
        }
    }
    else
    {
        QMessageBox::critical(this, tr("Error"),
            tr("Serial port already connected!"));
    }
}


/**
 * @brief MainWindow::connectSerialPort
 *
 * This method connect the application with the hardware serial interface.
 */
void
MainWindow::connectSerialPort ()
{
    if (!m_serialPort && m_serialSettings.name.isEmpty())
    {
        // enable serial connection:
        m_serialPort = new QSerialPort(this);
        m_serialPort->setPortName(m_serialSettings.name);
        if (m_serialPort->open(QIODevice::ReadWrite))
        {
            if (m_serialPort->setBaudRate(m_serialSettings.baudrate)
                    && m_serialPort->setDataBits(m_serialSettings.dataBits)
                    && m_serialPort->setParity(m_serialSettings.parity)
                    && m_serialPort->setStopBits(m_serialSettings.stopBits)
                    && m_serialPort->setFlowControl(m_serialSettings.flowControl))
            {
                // Change connect button and signal
                ui->serialConnect->setText("Disconnect");
                ui->serialConnect->setEnabled(true);
                disconnect(ui->serialConnect, SIGNAL(clicked()), this, SLOT(connectSerial()));
                connect(ui->serialConnect, SIGNAL(clicked()), this, SLOT(disconnectSerial()));

                // connect signals and activate buttons
                enableSendingButtons();

                // Update status line
                ui->serialStatus->setText(tr("Connected to %1 : %2, %3, %4, %5, %6")
                    .arg(m_serialSettings.name).arg(QString::number(m_serialSettings.baudrate))
                    .arg(m_serialSettings.dataBitsString).arg(m_serialSettings.parityString)
                    .arg(m_serialSettings.stopBitsString).arg(m_serialSettings.flowControlString));

            }
            else
            {
                m_serialPort->close();
                QMessageBox::critical(this, tr("Error"),
                    tr("Can't configure the serial port: %1,\nError code: %2")
                    .arg(m_serialSettings.name).arg(m_serialPort->error()));

                ui->serialStatus->setText(tr("Open error"));
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Error"),
                tr("Can't opened the serial port: %1,\nEerror code: %2")
                .arg(m_serialSettings.name).arg(m_serialPort->error()));

            ui->serialStatus->setText(tr("Configure error"));
        }
    }
    else if (!m_serialPort)
    {
        QMessageBox::critical(this, tr("Error"),
            tr("Serial port not yet specified. Setup it first!"));
    }
    else
    {
        QMessageBox::critical(this, tr("Error"),
            tr("Serial connection already active!"));
    }
}

/**
 * @brief MainWindow::disconnectSerialPort
 *
 * This method disconnect the application from the hardware serial interface.
 */
void MainWindow::disconnectSerialPort ()
{
    if (m_serialPort)
    {
        // Disable commuication port
        m_serialPort->close();

        // Disconnect comm signals
        //disconnect(m_commPort, SIGNAL(readyRead()), this, SLOT(readSerialData()));

        // Change connect button and signal
        ui->serialConnect->setText("Connect");
        ui->serialConnect->setEnabled(true);
        disconnect(ui->serialConnect, SIGNAL(clicked()), this, SLOT(disconnectSerial()));
        connect(ui->serialConnect, SIGNAL(clicked()), this, SLOT(connectSerial()));

        delete m_serialPort;
        m_serialPort = 0;

        // disconnect signals and deactivate buttons
        disableSendingButtons();

        ui->serialStatus->setText(tr("Disconnected"));
    }
    else
    {
        QMessageBox::critical(this, tr("Error"),
            tr("Serial connection not active!"));
    }
}

void MainWindow::initActionsConnections()
{
    connect(ui->serialSelect, SIGNAL(clicked()), this, SLOT(setupSerialPort()));
}

void MainWindow::enableSendingButtons()
{

}

void MainWindow::disableSendingButtons()
{

}
