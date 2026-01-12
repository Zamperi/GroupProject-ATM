#include "transactionwindow.h"
#include "ui_transactionwindow.h"
#include "windowmanager.h"
#include "datamanager.h"
#include "inactivitytimer.h"

transactionWindow::transactionWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::transactionWindow)
{
    ui->setupUi(this);
    //Signaalin yhdistys ja tokenin haku
    connect(&dataManager::instance(), &dataManager::transactionsFetched, this, &transactionWindow::updateTransactionHistory);
    QString token = dataManager::instance().getToken();
    dataManager::instance().fetchTransactions(token);

    //Taustakuvan asetus
    setupGraphicsView();

    //Labelin tyylin asetus
    ui->label_Transaction->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");

    //Buttonien tyylin asetus
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

    //Taulukon ulkonäön asetukset.
    QString tableStyle =
        "QTableWidget {"
        "    background-color: #2C3E50;"
        "    border: none;"
        "    gridline-color: #34495E;"
        "    color: white;"
        "    selection-background-color: #2980B9;"
        "}"
        "QHeaderView::section {"
        "    background-color: #34495E;"
        "    color: white;"
        "    padding: 5px;"
        "    border: 1px solid #2C3E50;"
        "    font-weight: bold;"
        "}"
        "QTableWidget::item {"
        "    padding: 8px;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #2980B9;"
        "}";

    ui->tableWidget->setStyleSheet(tableStyle);
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setColumnCount(3);
    QStringList headers = {"Account", "Withdraw", "Time"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->setColumnWidth(1, 300);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //Ajastimen asetukset
    connect(&InactivityTimer::instance(), &InactivityTimer::timeout, this, &transactionWindow::onInactivityTimeout);
    InactivityTimer::instance().startTimer(30000); //30 sekuntia.
}

transactionWindow::~transactionWindow()
{
    delete ui;
}

//Buttonien toiminnot
void transactionWindow::on_btnReturn_clicked()
{
    windowManager::instance().showMainWindow();
    this->close();
}


void transactionWindow::on_btnLogout_clicked()
{
    windowManager::instance().showStartWindow();
    this->close();
}

//Tilitapahtumien implemointi
void transactionWindow::updateTransactionHistory(QJsonArray transactions)
{
    ui->tableWidget->setRowCount(transactions.size());

    for (int i = 0; i < transactions.size(); ++i) {
        QJsonObject obj = transactions[i].toObject();
        QString account = QString::number(obj["idaccount"].toInt());
        QString amount = obj["amount"].toString();
        QString time = obj["time"].toString();

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(account));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(amount));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(time));
    }
}

//Aseta taustakuva
void transactionWindow::setupGraphicsView()
{
    QString imagePath = ":/images/ATM_background.png";
    QPixmap pixmap(imagePath);

    ui->label_Graphic->setPixmap(pixmap);
    ui->label_Graphic->setScaledContents(true); // Pakotetaan skaalautumaan
}

//Timerin toiminnot
//Suljetaan ikkuna, jos 30 sekuntia ilman käyttäjän inputtia.
void transactionWindow::onInactivityTimeout()
{
    qDebug()<<"Ikkuna sulkeutuu käyttämättömyyden vuoksi";
    windowManager::instance().showStartWindow();
    this->close(); // Sulje ikkuna
}

//Ajastimen nollaus.
void transactionWindow::resetInactivityTimer()
{
    InactivityTimer::instance().resetTimer();
}
