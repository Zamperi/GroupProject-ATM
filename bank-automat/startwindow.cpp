#include "ui_startwindow.h"
#include "windowmanager.h"

startWindow::startWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::startWindow)
{
    ui->setupUi(this);

    //Taustakuvan asetus
    setupGraphicsView();

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

    //Buttonin tyylin asetus
    ui->btnStart->setStyleSheet(buttonStyle);
}

startWindow::~startWindow()
{
    delete ui;

}

//Aloitusnapin painallus käynnistää kirjautumisen
void startWindow::on_btnStart_clicked()
{
    windowManager::instance().showLoginWindow();

    this->close();
}

void startWindow::setupGraphicsView()
{
    QString imagePath = ":/images/ATM_background.png";
    QPixmap pixmap(imagePath);

    ui->label_Graphic->setPixmap(pixmap);
    ui->label_Graphic->setScaledContents(true); // Pakotetaan skaalautumaan
}

