#include "withdrawothersum.h"
#include "ui_withdrawothersum.h"
#include "windowmanager.h"
#include "inactivitytimer.h"


withdrawOtherSum::withdrawOtherSum(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::withdrawOtherSum)
    , activeLineEdit(nullptr)
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

    QString buttonStyleOK = "QPushButton {"
                               "background-color: #27AE60;"
                               "border-radius: 10px;"
                               "font-size: 16px;"
                               "font-weight: bold;"
                               "color: white;"
                               "padding: 10px;"
                               "}"
                               "QPushButton:hover {"
                               "background-color: #229954;"
                               "}";

    QString buttonStyleClear = "QPushButton {"
                                "background-color: #F1C40F;"
                                "border-radius: 10px;"
                                "font-size: 16px;"
                                "font-weight: bold;"
                                "color: black;"
                                "padding: 10px;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: #D4AC0D;"
                                "}";

    //Buttonien tyylin asetus
    ui->btnLogout->setStyleSheet(buttonStyleLogOut);
    ui->btnReturn->setStyleSheet(buttonStyle);
    ui->buttonOK->setStyleSheet(buttonStyleOK);
    ui->buttonClear->setStyleSheet(buttonStyleClear);
    ui->button_0->setStyleSheet(buttonStyle);
    ui->button_1->setStyleSheet(buttonStyle);
    ui->button_2->setStyleSheet(buttonStyle);
    ui->button_3->setStyleSheet(buttonStyle);
    ui->button_4->setStyleSheet(buttonStyle);
    ui->button_5->setStyleSheet(buttonStyle);
    ui->button_6->setStyleSheet(buttonStyle);
    ui->button_7->setStyleSheet(buttonStyle);
    ui->button_8->setStyleSheet(buttonStyle);
    ui->button_9->setStyleSheet(buttonStyle);

    //Buttonien signaalien yhdistys
    connect(ui->button_0, &QPushButton::clicked, this, &withdrawOtherSum::onNumberButtonClicked);
    connect(ui->button_1, &QPushButton::clicked, this, &withdrawOtherSum::onNumberButtonClicked);
    connect(ui->button_2, &QPushButton::clicked, this, &withdrawOtherSum::onNumberButtonClicked);
    connect(ui->button_3, &QPushButton::clicked, this, &withdrawOtherSum::onNumberButtonClicked);
    connect(ui->button_4, &QPushButton::clicked, this, &withdrawOtherSum::onNumberButtonClicked);
    connect(ui->button_5, &QPushButton::clicked, this, &withdrawOtherSum::onNumberButtonClicked);
    connect(ui->button_6, &QPushButton::clicked, this, &withdrawOtherSum::onNumberButtonClicked);
    connect(ui->button_7, &QPushButton::clicked, this, &withdrawOtherSum::onNumberButtonClicked);
    connect(ui->button_8, &QPushButton::clicked, this, &withdrawOtherSum::onNumberButtonClicked);
    connect(ui->button_9, &QPushButton::clicked, this, &withdrawOtherSum::onNumberButtonClicked);

    // Yhdistä lineEditien signaalit
    connect(ui->lineEditWithdrawOtherSum, &CustomLineEdit::lineEditFocused, this, &withdrawOtherSum::onWithdrawOtherSumActive);

    activeLineEdit = ui->lineEditWithdrawOtherSum;

    //Ajastimen asetukset
    connect(&InactivityTimer::instance(), &InactivityTimer::timeout, this, &withdrawOtherSum::onInactivityTimeout);
    InactivityTimer::instance().startTimer(30000); //30 sekuntia.

}

withdrawOtherSum::~withdrawOtherSum()
{
    delete ui;
}

//Buttonien toimintojen määrittely
void withdrawOtherSum::onNumberButtonClicked()
{
    if (!activeLineEdit) {
        return;
    }

    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        QString currentText = activeLineEdit->text();
        currentText.append(button->text());
        activeLineEdit->setText(currentText);
    }

}

void withdrawOtherSum::on_buttonClear_clicked()
{
    qDebug()<<"Clear painettu";

    if (!activeLineEdit) {
        return;
    }

    activeLineEdit->clear();

}

void withdrawOtherSum::on_buttonOK_clicked()
{
    qDebug()<<"OK painettu";
    dataManager::instance().withdraw(ui->lineEditWithdrawOtherSum->text().toDouble());
    windowManager::instance().showWithdrawEndWindow();
    this->close();
}


//Tekstikenttien valinta
void withdrawOtherSum::onWithdrawOtherSumActive()
{
    qDebug()<<"Nostettava summa";
    activeLineEdit=ui->lineEditWithdrawOtherSum;
}


void withdrawOtherSum::on_btnLogout_clicked()
{
    windowManager::instance().showStartWindow();
    this->close();
}

void withdrawOtherSum::on_btnReturn_clicked()
{
    windowManager::instance().showMainWindow();
    this->close();
}

void withdrawOtherSum::setupGraphicsView()
{
    QString imagePath = ":/images/ATM_background.png";
    QPixmap pixmap(imagePath);

    ui->label_Graphic->setPixmap(pixmap);
    ui->label_Graphic->setScaledContents(true); // Pakotetaan skaalautumaan
}

//Timerin toiminnot
//Suljetaan ikkuna, jos 30 sekuntia ilman käyttäjän inputtia.
void withdrawOtherSum::onInactivityTimeout()
{
    qDebug()<<"Ikkuna sulkeutuu käyttämättömyyden vuoksi";
    windowManager::instance().showStartWindow();
    this->close(); // Sulje ikkuna
}

//Ajastimen nollaus.
void withdrawOtherSum::resetInactivityTimer()
{
    InactivityTimer::instance().resetTimer();
}
