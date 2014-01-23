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
 * @file serialsetup.cpp
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief Serial setup dialog class methods implementations.
 */

#include "serialsetup.h"
#include "ui_serialsetup.h"

/* TODO: Aggiungere controllo per parametri fissi di connessione! */

SerialSetup::SerialSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialSetup),
    m_isFixedParameter(false)

{
    ui->setupUi(this);

//    connect(ui->buttonBox, SIGNAL(clicked()),
//            this, SLOT(apply()));
    connect(ui->portsComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(showPortInfos(int)));

    // Set into combobox the list of available ports
    fillPortInfos();
    // Set into other combo box available parameters
    fillPortParameters();
    // Show parameters of first available port
    showPortInfos(0);

    // Update serial parameter choosed by the user
    updateSettings();
}

SerialSetup::~SerialSetup()
{
    delete ui;
}

void SerialSetup::fillPortInfos()
{
    // Clear combo box
    ui->portsComboBox->clear();
    // Scan and update combo box with the name of ports
    foreach (QSerialPortInfo info, QSerialPortInfo::availablePorts())
    {
        QStringList list;
        list << info.portName()
             << info.description()
             << info.manufacturer()
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString())
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : QString());

        ui->portsComboBox->addItem(list.first(), list);
    }
}

void SerialSetup::fillPortParameters()
{
    // fill data bits
    ui->dataBitsComboBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsComboBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsComboBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsComboBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsComboBox->setCurrentIndex(3);

    // fill parity
    ui->parityComboBox->addItem(QStringLiteral("None"), QSerialPort::NoParity);
    ui->parityComboBox->addItem(QStringLiteral("Even"), QSerialPort::EvenParity);
    ui->parityComboBox->addItem(QStringLiteral("Odd"), QSerialPort::OddParity);
    ui->parityComboBox->addItem(QStringLiteral("Mark"), QSerialPort::MarkParity);
    ui->parityComboBox->addItem(QStringLiteral("Space"), QSerialPort::SpaceParity);

    // fill stop bits
    ui->stopBitsComboBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsComboBox->addItem(QStringLiteral("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsComboBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    // fill flow control
    ui->flowControlComboBox->addItem(QStringLiteral("None"), QSerialPort::NoFlowControl);
    ui->flowControlComboBox->addItem(QStringLiteral("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlComboBox->addItem(QStringLiteral("XON/XOFF"), QSerialPort::SoftwareControl);
}

void SerialSetup::showPortInfos(int index)
{
    if (index != -1)
    {
        QStringList list = ui->portsComboBox->itemData(index).toStringList();

        if (list.size() > 0)
        {
            // Add each possible baudrate
            QSerialPortInfo portInfo(list.at(0));
            ui->rateComboBox->clear();
            foreach (quint32 rate, portInfo.standardBaudRates())
            {
                ui->rateComboBox->addItem(QString::number(rate));
            }
        }
    }
}

/**
 * @brief SerialSetup::setFixedParameters
 *
 * This method save fixed parameters and shows into combo box, moreover block
 * the use of these combo box from the user.
 *
 * @param rate Baudrate for the serial port
 * @param dataBits The number of data bits of serial port packet
 * @param flowControl The type of flow control
 * @param parity The type of parity used to check the packet
 * @param stopBits The number of stop bit into packet
 */
void SerialSetup::setFixedParameters(enum QSerialPort::BaudRate rate,
                        enum QSerialPort::DataBits dataBits,
                        enum QSerialPort::FlowControl flowControl,
                        enum QSerialPort::Parity parity,
                        enum QSerialPort::StopBits stopBits)
{
    m_isFixedParameter = true;
    /* TODO: save fixed parameters into settings */

    switch (dataBits)
    {
    case QSerialPort::Data5:
        ui->dataBitsComboBox->setCurrentIndex(0);
        break;
    case QSerialPort::Data6:
        ui->dataBitsComboBox->setCurrentIndex(1);
        break;
    case QSerialPort::Data7:
        ui->dataBitsComboBox->setCurrentIndex(2);
        break;
    case QSerialPort::Data8:
        ui->dataBitsComboBox->setCurrentIndex(3);
        break;
    default:
        ui->dataBitsComboBox->setCurrentIndex(3);
        break;
    }
    ui->dataBitsComboBox->setDisabled(true);

    switch (flowControl)
    {
    case QSerialPort::NoFlowControl:
        ui->flowControlComboBox->setCurrentIndex(0);
        break;
    case QSerialPort::HardwareControl:
        ui->flowControlComboBox->setCurrentIndex(1);
        break;
    case QSerialPort::SoftwareControl:
        ui->flowControlComboBox->setCurrentIndex(2);
        break;
    default:
        ui->flowControlComboBox->setCurrentIndex(0);
        break;
    }
    ui->flowControlComboBox->setDisabled(true);

    switch (parity)
    {
    case QSerialPort::NoParity:
        ui->parityComboBox->setCurrentIndex(0);
        break;
    case QSerialPort::EvenParity:
        ui->parityComboBox->setCurrentIndex(1);
        break;
    case QSerialPort::OddParity:
        ui->parityComboBox->setCurrentIndex(2);
        break;
    case QSerialPort::MarkParity:
        ui->parityComboBox->setCurrentIndex(3);
        break;
    case QSerialPort::SpaceParity:
        ui->parityComboBox->setCurrentIndex(4);
        break;
    default:
        ui->parityComboBox->setCurrentIndex(0);
        break;
    }
    ui->parityComboBox->setDisabled(true);

    switch (stopBits)
    {
    case QSerialPort::OneStop:
        ui->stopBitsComboBox->setCurrentIndex(0);
        break;
#ifdef Q_OS_WIN
    case QSerialPort::OneAndHalfStop:
        ui->stopBitsComboBox->setCurrentIndex(1);
        break;
    case QSerialPort::TwoStop:
        ui->stopBitsComboBox->setCurrentIndex(2);
        break;
#else
    case QSerialPort::TwoStop:
        ui->stopBitsComboBox->setCurrentIndex(1);
        break;
#endif
    default:
        ui->stopBitsComboBox->setCurrentIndex(0);
        break;
    }
    ui->stopBitsComboBox->setDisabled(true);

    ui->rateComboBox->clear();
    ui->rateComboBox->addItem(QString::number(rate));
    ui->rateComboBox->setDisabled(true);
}

/**
 * @brief SerialSetup::clearFixedParameters
 *
 * This method restores the ability to use the combo box to select the parameters.
 */
void SerialSetup::clearFixedParameters()
{
    m_isFixedParameter = false;

    ui->dataBitsComboBox->setDisabled(false);
    ui->flowControlComboBox->setDisabled(false);
    ui->parityComboBox->setDisabled(false);
    ui->stopBitsComboBox->setDisabled(false);
    ui->rateComboBox->setDisabled(false);
}

/**
 * @brief SerialSetup::updateSettings
 *
 * This method save into a SerialSettings object the settings selected by
 * the user.
 *
 */
void SerialSetup::updateSettings()
{
    m_settings.name = ui->portsComboBox->currentText();

    // Rate
    m_settings.baudrate = static_cast<QSerialPort::BaudRate>(
        ui->rateComboBox->itemData(ui->rateComboBox->currentIndex()).toInt());
    m_settings.baudrateString = QString::number(m_settings.baudrate);

    // Data bits
    m_settings.dataBits = static_cast<QSerialPort::DataBits>(
        ui->dataBitsComboBox->itemData(ui->dataBitsComboBox->currentIndex()).toInt());
    m_settings.dataBitsString = ui->dataBitsComboBox->currentText();

    // Parity
    m_settings.parity = static_cast<QSerialPort::Parity>(
        ui->parityComboBox->itemData(ui->parityComboBox->currentIndex()).toInt());
    m_settings.parityString = ui->parityComboBox->currentText();

    // Stop bits
    m_settings.stopBits = static_cast<QSerialPort::StopBits>(
                ui->stopBitsComboBox->itemData(ui->stopBitsComboBox->currentIndex()).toInt());
    m_settings.stopBitsString = ui->stopBitsComboBox->currentText();

    // Flow control
    m_settings.flowControl = static_cast<QSerialPort::FlowControl>(
        ui->flowControlComboBox->itemData(ui->flowControlComboBox->currentIndex()).toInt());
    m_settings.flowControlString = ui->flowControlComboBox->currentText();
}

/**
 * @brief SerialSetup::apply
 *
 * This method is a slot called by confirm button. It save the settings selected
 * by the user and close the dialog.
 */
void SerialSetup::apply()
{
    updateSettings();
    close();
}

/**
 * @brief SerialSetup::getSettings
 *
 * This method return the selected settings by the user.
 *
 * @return Selected settings by the user
 */
SerialSetup::SerialSettings SerialSetup::getSettings() const
{
    return m_settings;
}
