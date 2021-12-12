/*
 * File:    serialport.h
 * Author:  Alexander Schoefbaenker
 * Purpose: Declaration of the class SerialPort
 *
 * Created on 24. August 2021, 12:00
 */

// ---------------------------  Licences  ----------------------------- //

// From Qt-Example "Terminal"
// used Code in:
//  - Struct Settings
//  - Read/Write Port-functions
//  - handleError (partly)
//  - open/closePort-functions (partly)
//  - findPort-function (partly)

/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTime>
#include <QTimer>
#include <QDebug>


struct Settings {
    QString name;
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;
    QSerialPort::OpenMode mode;
};
struct SensorData {
    uint8_t accSenData, signalPower, tempOut, tempIn;
    uint16_t pressure;
};

#define DATA_SIZE 16
#define WAIT_TIL_TIMEOUT 10000
#define COM_PORT_NAME "COM3"
#define COUNT_ERROR 4
#define PORT_OPEN_ERROR 2

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);
    ~SerialPort();
    void initalizePort();
    void extractData();
    uint16_t getPressure()   { return sensordata.pressure; }
    uint8_t getaccSenData()  { return sensordata.accSenData; }
    uint8_t getSignalPower() { return sensordata.signalPower; }
    uint8_t getTempOut()     { return sensordata.tempOut; }
    uint8_t getTempIn()      { return sensordata.tempIn; }
    int16_t getDiff()        { return error; }

private slots:
    void findPort();
    void openPort();
    void closePort();
    void handleError(QSerialPort::SerialPortError error);
    void readData();
    const QString getTime();
    void writeData(const QString &data);
    void checkReceivedData();

signals:
    void dataReadytoGet();
    void connectionError();
    void portERROR();

private:    
    QSerialPort* port;
    Settings settings;
    SensorData sensordata;
    QSerialPortInfo ports;
    QList <QSerialPortInfo> portsAvailable;
    QString serialDataString;
    QStringList serialDataSplit;
    QByteArray serialData;
    QTime currentTime;
    bool receivedData, countError;
    uint8_t count;
    int16_t lastpressure;
    int16_t error; // Difference between the last- and current pressure measurement
};

#endif // SERIALPORT_H
