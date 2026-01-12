#include "withdrawwindow.h"
#include "ui_withdrawwindow.h"
#include "windowmanager.h"
#include "datamanager.h"
#include "inactivitytimer.h"

withdrawWindow::withdrawWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::withdrawWindow)
{
    ui->setupUi(this);

    //Aseta taustakuva
    setupGraphicsView();

    //Aseta labelin tyyli
    ui->labelWithdraw->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");

    //Aseta buttonien tyyli
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

    //Buttonien tyylin asetus
    ui->btnLogout->setStyleSheet(buttonStyleLogOut);
    ui->btnReturn->setStyleSheet(buttonStyle);
    ui->btn20->setStyleSheet(buttonStyle);
    ui->btn40->setStyleSheet(buttonStyle);
    ui->btn50->setStyleSheet(buttonStyle);
    ui->btn100->setStyleSheet(buttonStyle);
    ui->btnOtherSum->setStyleSheet(buttonStyle);

    //Ajastimen asetukset
    connect(&InactivityTimer::instance(), &InactivityTimer::timeout, this, &withdrawWindow::onInactivityTimeout);
    InactivityTimer::instance().startTimer(30000); //30 sekuntia.
}

withdrawWindow::~withdrawWindow()
{
    delete ui;
}


//Buttonien toiminnot
void withdrawWindow::on_btnLogout_clicked()
{
    windowManager::instance().showStartWindow();
    this->close();
}

void withdrawWindow::on_btnReturn_clicked()
{
    windowManager::instance().showMainWindow();
    this->close();
}

void withdrawWindow::on_btn20_clicked()
{
    dataManager::instance().withdraw(20);
    windowManager::instance().showWithdrawEndWindow();
    this->close();
}


void withdrawWindow::on_btn40_clicked()
{
    dataManager::instance().withdraw(40);
    windowManager::instance().showWithdrawEndWindow();
    this->close();
}

void withdrawWindow::on_btn50_clicked()
{
    windowManager::instance().showWithdrawEndWindow();
    dataManager::instance().withdraw(50);
    this->close();
}

void withdrawWindow::on_btn100_clicked()
{
    dataManager::instance().withdraw(100);
    windowManager::instance().showWithdrawEndWindow();
    this->close();
}

void withdrawWindow::on_btnOtherSum_clicked()
{
    windowManager::instance().showWithdrawOtherSumWindow();
    this->close();
}

//Aseta taustakuva
void withdrawWindow::setupGraphicsView()
{
    QString imagePath = ":/images/ATM_background.png";
    QPixmap pixmap(imagePath);

    ui->label_Graphic->setPixmap(pixmap);
    ui->label_Graphic->setScaledContents(true); // Pakotetaan skaalautumaan
}

//Timerin toiminnot
//Suljetaan ikkuna, jos 30 sekuntia ilman käyttäjän inputtia.
void withdrawWindow::onInactivityTimeout()
{
    qDebug()<<"Ikkuna sulkeutuu käyttämättömyyden vuoksi";
    windowManager::instance().showStartWindow();
    this->close(); // Sulje ikkuna
}

//Ajastimen nollaus.
void withdrawWindow::resetInactivityTimer()
{
    InactivityTimer::instance().resetTimer();
}
