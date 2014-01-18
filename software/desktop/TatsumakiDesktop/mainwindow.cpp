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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_serialPort(0)
{
    ui->setupUi(this);
    setWindowTitle(QString(PROJECT_NAME) + " v." + QString(PROJECT_VERSION));

    /* TODO: Disable sending buttons */

    m_serialSetup = new SerialSetup;
    /* Send fix data to serial interface! */

    ui->statusBar->showMessage(tr("No communication port connected!"));
}

MainWindow::~MainWindow()
{
    delete m_serialSetup;

    if (m_serialPort)
    {
        /* TODO: Close and delete! */
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
        m_serialSetup->exec();
//        setting = m_serialSetup->getParameters();
    }
    else
    {

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

}

/**
 * @brief MainWindow::disconnectSerialPort
 *
 * This method disconnect the application from the hardware serial interface.
 */
void
MainWindow::disconnectSerialPort ()
{

}
