/*
 * File:    serialport.cpp
 * Author:  Alexander Schoefbaenker
 * Purpose: Implementation of the class SerialPort
 *
 * Created on 24. August 2021, 12:00
 */

#include "serialport.h"

SerialPort::SerialPort(QObject *parent) : QObject(parent), port(new QSerialPort(this))
{
    // Initiate Variables
    receivedData = false;
    countError = false;
    count = 0;  
    error = 0;
    lastpressure = 0;

    // Connect Signals & Slots
    connect(port, &QSerialPort::readyRead, this, &SerialPort::readData);
    connect(port, &QSerialPort::errorOccurred, this, &SerialPort::handleError);
}

SerialPort::~SerialPort()
{
    delete port;
}

void SerialPort::initalizePort()
{
    // Port Suchen
    findPort();

    // Port-Settings
    settings.baudRate     = QSerialPort::Baud9600;
    settings.dataBits     = QSerialPort::Data8;
    settings.parity       = QSerialPort::NoParity;
    settings.stopBits     = QSerialPort::OneStop;
    settings.flowControl  = QSerialPort::NoFlowControl;
    settings.mode         = QSerialPort::ReadWrite;

    // Port öffnen
    openPort();

    // Daten anfragen
    // Uhrzeit senden
    if (port->isOpen()) writeData(getTime());
    else if (countError) {
        countError = false;
        emit portERROR();
    }
}

void SerialPort::extractData()
{
    serialDataSplit = serialDataString.split(";");

    sensordata.pressure    = serialDataSplit[0].toUInt(); // Druck
    sensordata.tempOut     = serialDataSplit[1].toUInt(); // Außentemp.
    sensordata.tempIn      = serialDataSplit[2].toUInt(); // Innentemp.
    sensordata.accSenData  = serialDataSplit[3].toUInt(); // Beschleunigungssensor
    sensordata.signalPower = serialDataSplit[4].toUInt(); // RSSI (Funk Verbindungsstärke)

    // Difference between last and current pressure measurement
    error = serialDataSplit[0].toInt() - lastpressure;
    lastpressure = serialDataSplit[0].toInt();

    emit dataReadytoGet();
}

void SerialPort::findPort()
{
    portsAvailable = ports.availablePorts();
    for (int i = 0; i < portsAvailable.length(); ++i) {
       ports = portsAvailable.at(i);
       if (ports.portName() == COM_PORT_NAME) {
           settings.name = ports.portName();
       }
       qDebug() << ports.portName();

    }
}

void SerialPort::openPort()
{
   if (!port->isOpen()) {
        port->setPortName(settings.name);
        port->setBaudRate(settings.baudRate);
        port->setDataBits(settings.dataBits);
        port->setParity(settings.parity);
        port->setStopBits(settings.stopBits);
        port->setFlowControl(settings.flowControl);

        qDebug() << "Opening Port...";
        port->open(settings.mode);
        if (port->isOpen()) qDebug() << "Port opened successfully";
    }
    else qDebug() << "Port is already opened";
}

void SerialPort::closePort()
{
    qDebug() << "Closing Port...";
    if (port->isOpen()) {
        port->close();
        if (!port->isOpen()) qDebug() << "Port closed successfully";
    }
    else qDebug() << "Port is already closed";
}

void SerialPort::handleError(QSerialPort::SerialPortError error)
{
    if (countError) {
        return;
    }
    if (error != QSerialPort::NoError) {
        countError = true;
    }
}

void SerialPort::readData()
{
    qDebug() << "Reading Data...";
    serialData = port->readAll();
    serialDataString += QString::fromStdString(serialData.toStdString());
    if (serialDataString.length() >= DATA_SIZE && serialDataString.endsWith(";")) {
        receivedData = true;
        qDebug() << serialDataString;
        extractData();
        serialDataString.clear();
        serialData.clear();
        qDebug() << "Data read successfully!";
    }
}

const QString SerialPort::getTime()
{
    QString curTime ="";
    currentTime = QTime::currentTime();
    curTime = currentTime.toString("HH;mm");
    qDebug() << curTime;
    return curTime;
}

void SerialPort::writeData(const QString &data)
{
    qDebug() << "Sending Message...";
    port->write(data.toLocal8Bit());
    QTimer::singleShot(WAIT_TIL_TIMEOUT, this, &SerialPort::checkReceivedData);
}

void SerialPort::checkReceivedData()
{
    if (!receivedData) {
        if (count == COUNT_ERROR) {
            count = 0;
            emit connectionError();
            return;
        }
        ++count;
    }
    // after 4 Times message to user
    else receivedData = false;
    // send request again
    writeData(getTime());
}
