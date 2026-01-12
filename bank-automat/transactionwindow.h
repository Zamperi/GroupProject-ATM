//Tämä luokka on tilitapahtumia varten

#ifndef TRANSACTIONWINDOW_H
#define TRANSACTIONWINDOW_H

#include <QDialog>

namespace Ui {
class transactionWindow;
}

class transactionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit transactionWindow(QWidget *parent = nullptr);
    ~transactionWindow();

private slots:
    void on_btnReturn_clicked();

    void on_btnLogout_clicked();

private:
    Ui::transactionWindow *ui;
    void updateTransactionHistory(QJsonArray transactions);
    void setupGraphicsView();
    QTimer *inactivityTimer;
    void resetInactivityTimer();
    void onInactivityTimeout();
};

#endif // TRANSACTIONWINDOW_H
