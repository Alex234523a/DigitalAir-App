/*
 * File:    mainwindow.h
 * Author:  Alexander Schoefbaenker
 * Purpose: Declaration of the class MainWindow
 *
 * Created on 24. August 2021, 12:00
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <serialport.h>
#include <QMessageBox>
#include <QPushButton>
#include <QAction>
#include <QShortcut>

#define FAKTOR 60
#define VOLUME_OF_BOTTLE_LITRES 6
#define CONNECTION_ERROR_MESSAGE "Connection-Error: App wird automatisch neugestartet."
#define UNKNOWN_ERROR_MESSAGE "Unknown-Error: Fehler beim Öffnen des Ports. Versuche \
                               den Port ernuet zu öffnen."

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
   static int const EXIT_CODE_REBOOT;

private slots:
    void initialize();
    void updateLCD();
    void connectError();
    void slotReboot();
    void portError();

private:
    Ui::MainWindow *ui;
    SerialPort *outgoing;
    QAction *rebootAct;
    void setSignalStrengthImg(uint8_t);
    uint8_t countPortErrors;
    uint16_t timeLeft;
};

#endif // MAINWINDOW_H
