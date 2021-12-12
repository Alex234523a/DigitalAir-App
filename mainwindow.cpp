/*
 * File:    mainwindow.cpp
 * Author:  Alexander Schoefbaenker
 * Purpose: Implementation of the class Mainwindow
 *
 * Created on 24. August 2021, 12:00
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

// To do:
// Innentemepratur zu hoch Warning ausgeben
// Is moving hinzufügen Label?
// Status informationen
// Design entsprechend auslegen

int const MainWindow::EXIT_CODE_REBOOT = -123456789;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      outgoing(new SerialPort(this))
{
    ui->setupUi(this);

    countPortErrors = 0;
    timeLeft = 0;

    connect(outgoing, &SerialPort::dataReadytoGet, this, &MainWindow::updateLCD);
    connect(outgoing, &SerialPort::connectionError, this, &MainWindow::connectError);
    connect(outgoing, &SerialPort::portERROR, this, &MainWindow::portError);

    //this->setWindowState(Qt::WindowMaximized);

    createActions();

    ui->pressureLCD->display("--");
    ui->tempALCD->display("--");
    ui->signalStrength->setPixmap(QPixmap(":/myresources/no-connection"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialize()
{
    ui->pressureLCD->display(0);
    ui->tempALCD->display(0);

    outgoing->initalizePort();
}

void MainWindow::updateLCD()
{
    ui->pressureLCD->display(outgoing->getPressure());
    ui->tempALCD->display(outgoing->getTempOut());
    setSignalStrengthImg(outgoing->getSignalPower());
    if (outgoing->getDiff() < 0) {
        timeLeft = (VOLUME_OF_BOTTLE_LITRES * outgoing->getPressure()) / FAKTOR;
        ui->timeLeftN_label->setNum(timeLeft);
        // Typ not right from uint16_t to int
        // This will be executed once!
    }
    // Else to calculate time left
}

void MainWindow::connectError()
{
    ui->signalStrength->setPixmap(QPixmap(":/myresources/no-connection"));
    statusBar()->showMessage(tr(CONNECTION_ERROR_MESSAGE), 3500);
    QTimer::singleShot(4000, this, &MainWindow::slotReboot);
}

void MainWindow::slotReboot()
{
    statusBar()->showMessage("Performing application reboot...");
    qApp->exit(MainWindow::EXIT_CODE_REBOOT);
}

void MainWindow::portError()
{
    statusBar()->showMessage(tr("Port-Error: Bitte starten sie die Basestation sowie die App neu."));
}

void MainWindow::createActions()
{
    //
}

void MainWindow::setSignalStrengthImg(uint8_t signalPower)
{
    if (signalPower < 50) {
        ui->signalStrength->setPixmap(QPixmap(":/myresources/excellent-connection"));
    } else if (signalPower < 60) {
        ui->signalStrength->setPixmap(QPixmap(":/myresources/good-connection"));
    } else if (signalPower < 70) {
        ui->signalStrength->setPixmap(QPixmap(":/myresources/fair-connection"));
    }else {
        ui->signalStrength->setPixmap(QPixmap(":/myresources/weak-connection"));
        //statusBar()->showMessage("Funkverbindung schwach", 2000);
    }
}


