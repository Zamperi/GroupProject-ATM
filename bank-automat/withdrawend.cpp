#include "withdrawend.h"
#include "ui_withdrawend.h"
#include "windowmanager.h"

withdrawEnd::withdrawEnd(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::withdrawEnd)
{
    ui->setupUi(this);

    setupGraphicsView();
    ui->labelWithdraw->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");
     ui->labelWithdraw_2->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");


    inactivityTimer = new QTimer(this);
    connect(inactivityTimer, &QTimer::timeout, this, &withdrawEnd::onTimeout);
    inactivityTimer->start(5000);//5 sekuntia.

    connect(&dataManager::instance(), &dataManager::withdrawSucceeded, this, &withdrawEnd::updateBalance);
    connect(&dataManager::instance(), &dataManager::withdrawFailed, this, &withdrawEnd::withdrawFailed);
}

withdrawEnd::~withdrawEnd()
{
    delete ui;
}

//Timerin toiminnot
//Suljetaan ikkuna 5 sekunnin jälkeen
void withdrawEnd::onTimeout()
{
    qDebug()<<"Rahan nosto suljetaan";
    windowManager::instance().showStartWindow();
    this->close();
}

//Ajastimen nollaus
void withdrawEnd::resetTimer()
{
    inactivityTimer->stop();
    inactivityTimer->start(10000);
}

void withdrawEnd::updateBalance(QString inputAmount)
{
    ui->labelWithdraw->setText(QString("New balance: %1").arg(inputAmount));
}

void withdrawEnd::withdrawFailed(QString errorMessage)
{
    ui->labelWithdraw->setText(errorMessage);
}

//Aseta taustakuva
void withdrawEnd::setupGraphicsView()
{
    QString imagePath = ":/images/ATM_background.png";
    QPixmap pixmap(imagePath);

    ui->label_Graphic->setPixmap(pixmap);
    ui->label_Graphic->setScaledContents(true); // Pakotetaan skaalautumaan
}
