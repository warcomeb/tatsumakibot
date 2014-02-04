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
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_serialPort(0),
    m_isSerialConnected(false),
    m_speedUp(false),
    m_keyDirection(Qt::Key_unknown)
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
    m_serialBusStatus = Protocol::Free;

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
        switch (e->key())
        {
        case Qt::Key_Up:
        case Qt::Key_W:
            m_keyDirection = Qt::Key_Up;

            ui->downButton->setChecked(false);
            ui->downMoreButton->setChecked(false);
            ui->rightButton->setChecked(false);
            ui->rightMoreButton->setChecked(false);
            ui->leftButton->setChecked(false);
            ui->leftMoreButton->setChecked(false);

            if (!m_speedUp)
            {
                ui->upButton->setChecked(true);
                ui->upMoreButton->setChecked(false);
            }
            else
            {
                ui->upButton->setChecked(false);
                ui->upMoreButton->setChecked(true);
            }
            break;
        case Qt::Key_Down:
        case Qt::Key_Z:
            m_keyDirection = Qt::Key_Down;

            ui->upButton->setChecked(false);
            ui->upMoreButton->setChecked(false);
            ui->rightButton->setChecked(false);
            ui->rightMoreButton->setChecked(false);
            ui->leftButton->setChecked(false);
            ui->leftMoreButton->setChecked(false);

            if (!m_speedUp)
            {
                ui->downButton->setChecked(true);
                ui->downMoreButton->setChecked(false);
            }
            else
            {
                ui->downButton->setChecked(false);
                ui->downMoreButton->setChecked(true);
            }
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            m_keyDirection = Qt::Key_Right;

            ui->upButton->setChecked(false);
            ui->upMoreButton->setChecked(false);
            ui->downButton->setChecked(false);
            ui->downMoreButton->setChecked(false);
            ui->leftButton->setChecked(false);
            ui->leftMoreButton->setChecked(false);

            if (!m_speedUp)
            {
                ui->rightButton->setChecked(true);
                ui->rightMoreButton->setChecked(false);
            }
            else
            {
                ui->rightButton->setChecked(false);
                ui->rightMoreButton->setChecked(true);
            }
            break;
        case Qt::Key_Left:
        case Qt::Key_A:
            m_keyDirection = Qt::Key_Left;

            ui->upButton->setChecked(false);
            ui->upMoreButton->setChecked(false);
            ui->downButton->setChecked(false);
            ui->downMoreButton->setChecked(false);
            ui->rightButton->setChecked(false);
            ui->rightMoreButton->setChecked(false);

            if (!m_speedUp)
            {
                ui->leftButton->setChecked(true);
                ui->leftMoreButton->setChecked(false);
            }
            else
            {
                ui->leftButton->setChecked(false);
                ui->leftMoreButton->setChecked(true);
            }
            break;
        case Qt::Key_Shift:
            qDebug() << "shift checked!";
            m_speedUp = true;
            switch (m_keyDirection)
            {
            case Qt::Key_Up:
            case Qt::Key_W:
                ui->upButton->setChecked(false);
                ui->upMoreButton->setChecked(true);
                break;
            case Qt::Key_Down:
            case Qt::Key_Z:
                ui->downButton->setChecked(false);
                ui->downMoreButton->setChecked(true);
                break;
            case Qt::Key_Right:
            case Qt::Key_D:
                ui->rightButton->setChecked(false);
                ui->rightMoreButton->setChecked(true);
                break;
            case Qt::Key_Left:
            case Qt::Key_A:
                ui->leftButton->setChecked(false);
                ui->leftMoreButton->setChecked(true);
                break;
            default:
                // Nothing
                break;
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

void MainWindow::keyReleaseEvent(QKeyEvent* e)
{
    if (m_isSerialConnected)
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
            m_keyDirection = Qt::Key_unknown;

            ui->upButton->setChecked(false);
            ui->upMoreButton->setChecked(false);
            ui->downButton->setChecked(false);
            ui->downMoreButton->setChecked(false);
            ui->rightButton->setChecked(false);
            ui->rightMoreButton->setChecked(false);
            ui->leftButton->setChecked(false);
            ui->leftMoreButton->setChecked(false);

            break;
        case Qt::Key_Shift:
            m_speedUp = false;

            switch (m_keyDirection)
            {
            case Qt::Key_Up:
            case Qt::Key_W:
                ui->upButton->setChecked(true);
                ui->upMoreButton->setChecked(false);
                break;
            case Qt::Key_Down:
            case Qt::Key_Z:
                ui->downButton->setChecked(true);
                ui->downMoreButton->setChecked(false);
                break;
            case Qt::Key_Right:
            case Qt::Key_D:
                ui->rightButton->setChecked(true);
                ui->rightMoreButton->setChecked(false);
                break;
            case Qt::Key_Left:
            case Qt::Key_A:
                ui->leftButton->setChecked(true);
                ui->leftMoreButton->setChecked(false);
                break;
            default:
                // Nothing
                break;
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
            qDebug() << "Port " << m_serialSettings.name << " "
                     << m_serialSettings.baudrate << "-" << m_serialSettings.dataBits << "-"
                     << m_serialSettings.parity << "-" << m_serialSettings.stopBits << "-"
                     << m_serialSettings.flowControl;

            // WARNING: no problem for warning message
            // https://qt-project.org/forums/viewthread/36296
            if (m_serialPort->setBaudRate(m_serialSettings.baudrate)
                    && m_serialPort->setDataBits(m_serialSettings.dataBits)
                    && m_serialPort->setParity(m_serialSettings.parity)
                    && m_serialPort->setStopBits(m_serialSettings.stopBits)
                    && m_serialPort->setFlowControl(m_serialSettings.flowControl))
            {
                m_isSerialConnected = true;

                // Change connect button and signal
                ui->serialConnect->setText("Disconnect");
                ui->serialConnect->setEnabled(true);
                disconnect(ui->serialConnect, SIGNAL(clicked()), this, SLOT(connectSerialPort()));
                connect(ui->serialConnect, SIGNAL(clicked()), this, SLOT(disconnectSerialPort()));

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
        //disconnect(m_commPort, SIGNAL(readyRead()), this, SLOT(readSerialData()));

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
//    if ((sender() == m_updateTimer) && (m_busStatus == Protocol::Free))
//    {
//        if (m_activeNode.size() > 0)
//        {
//            m_remainingNode = m_activeNode.size();
//            m_currentNode   = 0;

//            m_message.command = Protocol::KeepAlive;
//            m_message.boardId = m_activeNode[m_currentNode];
//        }
//        else
//        {
//            return;
//        }
//    }
//    else if ((sender() == ui->sendRoutine) || (sender() == ui->sendAlert))
//    {

//    }

//    m_busStatus = Protocol::MessageDelivery;

//    QByteArray messageArray;
//    Protocol::composeMessage(m_message,messageArray);
//    m_commPort->write(messageArray);

//    /* TODO: debug code! */

//    /* TODO: un timeout lo facciamo partire di 2-3s? */

//    ui->status->setText("Waiting reply...");

//    m_busStatus = Protocol::WaintingReply;
}

void MainWindow::receiveSerialData()
{

}

void MainWindow::enableSendingButtons()
{

}

void MainWindow::disableSendingButtons()
{

}
