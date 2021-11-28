/*
 * File:    main.cpp
 * Author:  Alexander Schoefbaenker
 * Purpose: Main-File
 *
 * Created on 24. August 2021, 12:00
 */

#include "mainwindow.h"
#include <QSplashScreen>
#include <QApplication>
#include <QTimer>
#include <QPixmap>

int main(int argc, char *argv[])
{
    int currentExitCode = 0;
    do {
        QApplication a(argc, argv);
        QPixmap pixmap(":/myresources/flam.png");
        QSplashScreen splashScreen(pixmap);
        MainWindow w;
        splashScreen.show();
        splashScreen.showMessage("Loading ...",Qt::AlignBottom,Qt::black);
        QTimer::singleShot(1500, &w, SLOT(show())); // adjust time
        QTimer::singleShot(2000, &w, SLOT(initialize()));
        splashScreen.finish(&w);
        currentExitCode = a.exec();
    } while(currentExitCode == MainWindow::EXIT_CODE_REBOOT);

    return currentExitCode;
}
