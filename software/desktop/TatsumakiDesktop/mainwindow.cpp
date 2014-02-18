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
#include "protocol.h"

#include <QMessageBox>
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_serialPort(0),
    m_isSerialConnected(false),
    m_speedUp(false)
{
    ui->setupUi(this);
    setWindowTitle(QString(PROJECT_NAME) + " v." + QString(PROJECT_VERSION));

    setFocusPolicy(Qt::StrongFocus);

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
    m_serialBusStatus = Protocol::Free;

    connect(ui->normalSpeedSlider, SIGNAL(valueChanged(int)),
            ui->normalSpeedLcd, SLOT(display(int)));
    ui->normalSpeedLcd->display(ui->normalSpeedSlider->value());
    connect(ui->highSpeedSlider, SIGNAL(valueChanged(int)),
            ui->highSpeedLcd, SLOT(display(int)));
    ui->highSpeedLcd->display(ui->highSpeedSlider->value());

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

void MainWindow::keyPressEvent(QKeyEvent* e)
{
    if (m_isSerialConnected)
    {
        if(!e->isAutoRepeat())
        {
            switch (e->key())
            {
            case Qt::Key_Up:
            case Qt::Key_W:
                m_message.speed = (m_speedUp) ? (qint8) ui->highSpeedSlider->value() :
                                                (qint8) ui->normalSpeedSlider->value();
                m_message.direction = Protocol::MoveUp;
                m_message.command = Protocol::RobotMove;

                ui->downButton->setChecked(false);
                ui->rightButton->setChecked(false);
                ui->leftButton->setChecked(false);

                ui->upButton->setChecked(true);
                sendSerialData();
                break;
            case Qt::Key_Down:
            case Qt::Key_Z:
                m_message.speed = (m_speedUp) ? (qint8) ui->highSpeedSlider->value() :
                                                (qint8) ui->normalSpeedSlider->value();
                m_message.direction = Protocol::MoveDown;
                m_message.command = Protocol::RobotMove;

                ui->upButton->setChecked(false);
                ui->rightButton->setChecked(false);
                ui->leftButton->setChecked(false);

                ui->downButton->setChecked(true);
                sendSerialData();
                break;
            case Qt::Key_Right:
            case Qt::Key_D:
                m_message.speed = (m_speedUp) ? (qint8) ui->highSpeedSlider->value() :
                                                (qint8) ui->normalSpeedSlider->value();
                m_message.direction = Protocol::MoveRight;
                m_message.command = Protocol::RobotMove;

                ui->upButton->setChecked(false);
                ui->downButton->setChecked(false);
                ui->leftButton->setChecked(false);

                ui->rightButton->setChecked(true);
                sendSerialData();
                break;
            case Qt::Key_Left:
            case Qt::Key_A:
                m_message.speed = (m_speedUp) ? (qint8) ui->highSpeedSlider->value() :
                                                (qint8) ui->normalSpeedSlider->value();
                m_message.direction = Protocol::MoveLeft;
                m_message.command = Protocol::RobotMove;

                ui->upButton->setChecked(false);
                ui->downButton->setChecked(false);
                ui->rightButton->setChecked(false);

                ui->leftButton->setChecked(true);
                sendSerialData();
                break;
            case Qt::Key_Shift:
                m_speedUp = true;
                ui->SpeedUpButton->setChecked(true);

                if ((m_message.direction != Protocol::MoveNone) && (m_message.speed > 0))
                {
                    m_message.speed = ui->highSpeedSlider->value();
                    m_message.command = Protocol::RobotMove;
                    sendSerialData();
                }
                break;
            case Qt::Key_Space:
            case Qt::Key_S:
                break;
            default:
                // Nothing
                break;
            }
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* e)
{
    if (m_isSerialConnected)
    {
        if(!e->isAutoRepeat())
        {
            switch (e->key())
            {
            case Qt::Key_Up:
            case Qt::Key_W:

            case Qt::Key_Down:
            case Qt::Key_Z:

            case Qt::Key_Right:
            case Qt::Key_D:

            case Qt::Key_Left:
            case Qt::Key_A:
                m_message.direction = Protocol::MoveNone;
                m_message.speed = 0;
                m_message.command = Protocol::RobotMove;

                ui->upButton->setChecked(false);
                ui->downButton->setChecked(false);
                ui->rightButton->setChecked(false);
                ui->leftButton->setChecked(false);

                sendSerialData();
                break;
            case Qt::Key_Shift:
                m_speedUp = false;
                ui->SpeedUpButton->setChecked(false);

                if ((m_message.direction != Protocol::MoveNone) && (m_message.speed > 0))
                {
                    m_message.speed = ui->normalSpeedSlider->value();
                    m_message.command = Protocol::RobotMove;
                    sendSerialData();
                }
                break;
            case Qt::Key_Space:
            case Qt::Key_S:
                break;
            default:
                // Nothing
                break;
            }
        }
    }
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

        if (QString::compare(QString("No Port"),m_serialSettings.name) == 0)
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
    if ((!m_serialPort || !m_serialPort->isOpen()) &&
        (QString::compare(QString("No Port"),m_serialSettings.name)!=0))
    {
        // enable serial connection:
        m_serialPort = new QSerialPort(this);
        m_serialPort->setPortName(m_serialSettings.name);
        if (m_serialPort->open(QIODevice::ReadWrite))
        {
            // WARNING: no problem for warning message
            // https://qt-project.org/forums/viewthread/36296
            if (m_serialPort->setBaudRate(m_serialSettings.baudrate)
                    && m_serialPort->setDataBits(m_serialSettings.dataBits)
                    && m_serialPort->setParity(m_serialSettings.parity)
                    && m_serialPort->setStopBits(m_serialSettings.stopBits)
                    && m_serialPort->setFlowControl(m_serialSettings.flowControl))
            {
                qDebug() << "Serial connect:" << m_serialSettings.name << "-"
                         << m_serialSettings.baudrate << "-" << m_serialSettings.dataBits << "-"
                         << m_serialSettings.parity << "-" << m_serialSettings.stopBits << "-"
                         << m_serialSettings.flowControl;

                m_isSerialConnected = true;

                // Change connect button and signal
                ui->serialConnect->setText("Disconnect");
                ui->serialConnect->setEnabled(true);
                disconnect(ui->serialConnect, SIGNAL(clicked()), this, SLOT(connectSerialPort()));
                connect(ui->serialConnect, SIGNAL(clicked()), this, SLOT(disconnectSerialPort()));

                connect(m_serialPort, SIGNAL(readyRead()), this, SLOT(receiveSerialData()));

                // connect signals and activate buttons
                enableSendingButtons();

                // Update status line
                ui->serialStatus->setText(tr("%1: %2-%3-%4-%5-%6")
                    .arg(m_serialSettings.name).arg(QString::number(m_serialSettings.baudrate))
                    .arg(m_serialSettings.dataBitsString).arg(m_serialSettings.parityString)
                    .arg(m_serialSettings.stopBitsString).arg(m_serialSettings.flowControlString));

            }
            else
            {
                m_serialPort->close();

                QMessageBox::critical(this, tr("Error"),
                    tr("Can't configure the serial port: %1,\nError code: %2")
                    .arg(m_serialSettings.name).arg(m_serialPort->errorString()));

                ui->serialStatus->setText(tr("Open error"));
            }
        }
        else
        {
            m_serialPort->close();


            QMessageBox::critical(this, tr("Error"),
                tr("Can't opened the serial port: %1,\nEerror code: %2")
                .arg(m_serialSettings.name).arg(m_serialPort->errorString()));

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
        m_isSerialConnected = false;


        // Disconnect comm signals
        disconnect(m_serialPort, SIGNAL(readyRead()), this, SLOT(receiveSerialData()));

        // Change connect button and signal
        ui->serialConnect->setText("Connect");
        ui->serialConnect->setEnabled(true);
        disconnect(ui->serialConnect, SIGNAL(clicked()), this, SLOT(disconnectSerialPort()));
        connect(ui->serialConnect, SIGNAL(clicked()), this, SLOT(connectSerialPort()));

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

void MainWindow::sendSerialData()
{
//    if (m_serialBusStatus == Protocol::Free)
//    {
        m_serialBusStatus = Protocol::MessageDelivery;

        QByteArray messageArray;
        QString message;
        Protocol::composeMessage(m_message,messageArray);
        Protocol::messageToString(messageArray,message);
        m_serialPort->write(messageArray);

        // debug code!
        qDebug() << "Serial send message:" << message;

        /* TODO: Timeout of 2-3s? */

        ui->status->setText("Waiting reply...");
        qDebug() << "Serial waiting reply...";

        m_serialBusStatus = Protocol::WaintingReply;
//    }
}

void MainWindow::receiveSerialData()
{
    if ((m_serialBusStatus == Protocol::WaintingReply) ||
        (m_serialBusStatus == Protocol::MessageReception))
    {
        // Save new byte stream
        if (m_serialBusStatus == Protocol::WaintingReply)
        {
            m_serialReceiveBuffer = m_serialPort->readAll();
            m_serialBusStatus = Protocol::MessageReception;
        }
        else
        {
            m_serialReceiveBuffer.append(m_serialPort->readAll());
        }

        if (m_serialReceiveBuffer.endsWith(Protocol::messageStop))
        {
            m_serialBusStatus = Protocol::MessageReceived;

            /* Print reply message */
            QString replyMessage;
            Protocol::messageToString(m_serialReceiveBuffer,replyMessage);

            ui->status->setText("Message received!");
            qDebug() << "Serial receive message:" << replyMessage;

            if (!Protocol::controlReceivedMessage(m_message,m_serialReceiveBuffer))
            {
                ui->status->setText("Message not compliant!");
                qDebug() << "Serial receive message not compliant!";
            }
            else
            {
                // Parse Message
                Protocol::parseReceivedMessage(m_serialReceiveBuffer,m_replyMessage);
                // TODO: do action from serial message!
            }

            m_serialBusStatus = Protocol::Free;
        }
    }
    else
    {
        ui->status->setText("The message is out of date!");
        qDebug() << "Serial receive message is out of date: " <<
                    m_serialPort->readAll();
    }
}

void MainWindow::enableSendingButtons()
{

}

void MainWindow::disableSendingButtons()
{

}
