#include "balancewindow.h"
#include "ui_balancewindow.h"
#include "inactivitytimer.h"

BalanceWindow::BalanceWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BalanceWindow)
{
    ui->setupUi(this);

    //Grafiikan ja nappuloiden asetus
    setupGraphicsView();
    ui->labelType->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");
    ui->labelBalanceCredit->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");
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

    //Buttonien värien asetus.
    ui->btnLogout->setStyleSheet(buttonStyleLogOut);
    ui->btnReturn->setStyleSheet(buttonStyle);

    connectDataManager();
    dataManager::instance().fetchBalance();

    //Ajastimen asetukset
    connect(&InactivityTimer::instance(), &InactivityTimer::timeout, this, &BalanceWindow::onInactivityTimeout);
    InactivityTimer::instance().startTimer(30000); //30 sekuntia.
}

void BalanceWindow::connectDataManager()
{
    connect(&dataManager::instance(), &dataManager::fetchedBalance,
            this, &BalanceWindow::setLabels);
}

BalanceWindow::~BalanceWindow()
{
    delete ui;
}

void BalanceWindow::setLabels(float balance, float creditLimit, int typeCredit)
{
    ui->labelType->setAlignment(Qt::AlignHCenter);
    ui->labelBalanceCredit->setAlignment(Qt::AlignHCenter);
    if(typeCredit == 1) {
        ui->labelType->setText("Usable credit:");
        double usableCredit = creditLimit + balance;
        ui->labelBalanceCredit->setText((QString::number(usableCredit, 'f', 2)) + "€");
    } else {
        ui->labelType->setText("Balance:");
        ui->labelBalanceCredit->setText((QString::number(balance, 'f', 2)) + "€");
    }
}

void BalanceWindow::on_btnReturn_clicked()
{
    windowManager::instance().showMainWindow();
    this->close();
}

void BalanceWindow::on_btnLogout_clicked()
{
    windowManager::instance().showStartWindow();
    this->close();
}

void BalanceWindow::setupGraphicsView()
{
    QString imagePath = ":/images/ATM_background.png";
    QPixmap pixmap(imagePath);

    ui->label_Graphic->setPixmap(pixmap);
    ui->label_Graphic->setScaledContents(true); // Pakotetaan skaalautumaan
}

//Suljetaan ikkuna, jos 10 sekuntia ilman käyttäjän inputtia.
void BalanceWindow::onInactivityTimeout()
{
    qDebug()<<"Ikkuna sulkeutuu käyttämättömyyden vuoksi";
    windowManager::instance().showStartWindow();
    this->close(); // Sulje ikkuna
}

//Ajastimen nollaus
void BalanceWindow::resetInactivityTimer()
{
    InactivityTimer::instance().resetTimer();
}
