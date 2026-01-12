#include "environment.h"
#include "login.h"
#include "ui_login.h"
#include "windowmanager.h"
#include "datamanager.h"
#include "inactivitytimer.h"

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
    , activeLineEdit(nullptr)
{
    ui->setupUi(this);

    //Taustakuvan asetus
    setupGraphicsView();

    //Backendin viestien labelin tyylin määritys.
    ui->labelError->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");

    //Buttonien visuaaliset määritykset
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

    //Buttonien tyylien asetus.
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
    connect(ui->button_0, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button_1, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button_2, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button_3, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button_4, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button_5, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button_6, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button_7, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button_8, &QPushButton::clicked, this, &Login::onNumberButtonClicked);
    connect(ui->button_9, &QPushButton::clicked, this, &Login::onNumberButtonClicked);

    // LineEdittien signaalien yhdistys
    connect(ui->lineEditCardNum, &CustomLineEdit::lineEditFocused, this, &Login::onSetCardNumberActive);
    connect(ui->lineEditPin, &CustomLineEdit::lineEditFocused, this, &Login::onSetPinActive);

    connect(&dataManager::instance(), &dataManager::loginSucceeded, this, &Login::loginSuccess);
    connect(&dataManager::instance(), &dataManager::loginFailed, this, &Login::loginFailure);

    activeLineEdit = ui->lineEditCardNum; // Kortin syöttö oletukseksi

    connect(&InactivityTimer::instance(), &InactivityTimer::timeout, this, &Login::onInactivityTimeout);
    InactivityTimer::instance().startTimer(10000);
}

Login::~Login()
{
    delete ui;
}

//Ruudun nappien toimintojen määrittely
void Login::onNumberButtonClicked()
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

    resetInactivityTimer();
}

void Login::on_buttonClear_clicked()
{
    if (!activeLineEdit) {
        return;
    }

    activeLineEdit->clear();

    resetInactivityTimer();
}

void Login::on_buttonOK_clicked()
{
    resetInactivityTimer();
    dataManager::instance().attemptLogin(ui->lineEditCardNum->text(),ui->lineEditPin->text());
}


//Tekstikenttien valinta
void Login::onSetCardNumberActive()
{
    activeLineEdit=ui->lineEditCardNum;
}

void Login::onSetPinActive()
{
    activeLineEdit=ui->lineEditPin;
}

//Timerin toiminnot
//Suljetaan ikkuna, jos 10 sekuntia ilman käyttäjän inputtia
void Login::onInactivityTimeout()
{
    qDebug()<<"Login sulkeutuu käyttämättömyyden vuoksi";
    windowManager::instance().showStartWindow();
    this->close(); // Sulje login-ikkuna
}

//Ajastimen nollaus
void Login::resetInactivityTimer()
{
    InactivityTimer::instance().resetTimer();
}

//Kirjautumisen hallinta
void Login::loginSuccess()
{
    qDebug() << "Login onnistui!";
    ui->labelError->clear();
    windowManager::instance().showMainWindow();
    this->accept();
}

void Login::loginFailure(QString errorMessage)
{
    qDebug() << "Login epäonnistui!";
    ui->labelError->setText(errorMessage);
}

//Taustakuvan asetus
void Login::setupGraphicsView()
{
    QString imagePath = ":/images/ATM_background.png";
    QPixmap pixmap(imagePath);

    ui->label_Graphic->setPixmap(pixmap);
    ui->label_Graphic->setScaledContents(true); // Pakotetaan skaalautumaan
}
