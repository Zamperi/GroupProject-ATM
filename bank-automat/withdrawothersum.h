//Tämä luokka on muun rahasumman nostoa varten

#ifndef WITHDRAWOTHERSUM_H
#define WITHDRAWOTHERSUM_H

#include <QDialog>
#include "datamanager.h"
#include <qlineedit.h>

namespace Ui {
class withdrawOtherSum;
}

class withdrawOtherSum : public QDialog
{
    Q_OBJECT

public:
    explicit withdrawOtherSum(QWidget *parent = nullptr);
    ~withdrawOtherSum();

private slots:
    void on_btnReturn_clicked();
    void on_btnLogout_clicked();
    void on_buttonOK_clicked();
    void onNumberButtonClicked();
    void onWithdrawOtherSumActive();
    void on_buttonClear_clicked();

private:
    Ui::withdrawOtherSum *ui;
    QLineEdit *activeLineEdit;
    void setupGraphicsView();
    QTimer *inactivityTimer;
    void resetInactivityTimer();
    void onInactivityTimeout();
};

#endif // WITHDRAWOTHERSUM_H
