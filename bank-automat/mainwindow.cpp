#include "ui_mainwindow.h"
#include "windowmanager.h"
#include "datamanager.h"
#include <QDebug>
#include "inactivitytimer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Taustakuvan asetus
    setupGraphicsView();

    //Labelin tyylin määritys
    ui->labelWlcome->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");

    //Buttonien tyylin määritys
    QString buttonStyle = "QPushButton {"
                          "background-color: #3498db;"
                          "border-radius: 10px;"
                          "font-size: 16px;"
                          "font-weight: bold;"
                          "color: white;"
                          "padding: 10px;"
                          "}"
                          "QPushButton:hover {"
                          "background-color: #2980b9;"
                          "}";

    QString buttonStyleLogOut = "QPushButton {"
                                "background-color: #C0392B;"
                                "border-radius: 10px;"
                                "font-size: 16px;"
                                "font-weight: bold;"
                                "color: white;"
                                "padding: 10px;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: #E74C3C;"
                                "}";

    //Buttonien värit
    ui->btnLogOut->setStyleSheet(buttonStyleLogOut);
    ui->btnShowBalance->setStyleSheet(buttonStyle);
    ui->btnWithdraw->setStyleSheet(buttonStyle);
    ui->btnTransactions->setStyleSheet(buttonStyle);

    QString token = dataManager::instance().getToken();
    qDebug()<<"Token:"<<token;

    //Ajastimen asetukset
    connect(&InactivityTimer::instance(), &InactivityTimer::timeout, this, &MainWindow::onInactivityTimeout);
    InactivityTimer::instance().startTimer(30000); //30 sekuntia.
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Uloskirjautuminen
void MainWindow::on_btnLogOut_clicked()
{
    windowManager::instance().showStartWindow();
    this->close();
}

//Näytä tilin saldo
void MainWindow::on_btnShowBalance_clicked()
{
    windowManager::instance().showBalanceWindow();
    this->close(); // sulje tämänhetkinen ikkuna
}

//Nosta rahaa
void MainWindow::on_btnWithdraw_clicked()
{
    qDebug()<<"Nosta rahaa";
    windowManager::instance().showWithdrawWindow();
    this->close();
}

//Näytä tilitapahtumat
void MainWindow::on_btnTransactions_clicked()
{
    qDebug()<<"Näytä tilitapahtumat";
    windowManager::instance().showTransactionWindow();
    this->close();
}

void MainWindow::setupGraphicsView()
{
    QString imagePath = ":/images/ATM_background.png";
    QPixmap pixmap(imagePath);

    ui->label_Graphic->setPixmap(pixmap);
    ui->label_Graphic->setScaledContents(true); // Pakotetaan skaalautumaan
}

//Timerin toiminnot
//Suljetaan ikkuna, jos 30 sekuntia ilman käyttäjän inputtia
void MainWindow::onInactivityTimeout()
{
    qDebug()<<"Ikkuna sulkeutuu käyttämättömyyden vuoksi";
    windowManager::instance().showStartWindow();
    this->close(); // Sulje ikkuna
}

//Ajastimen nollaus
void MainWindow::resetInactivityTimer()
{
    InactivityTimer::instance().resetTimer();
}
