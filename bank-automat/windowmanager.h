//Tämä luokka on ikkunoiden hallintaa varten

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>
#include <QPointer>
#include "startwindow.h"
#include "mainwindow.h"
#include "login.h"
#include "transactionwindow.h"
#include "withdrawwindow.h"
#include "withdrawothersum.h"
#include "withdrawend.h"
#include "balancewindow.h"

class windowManager : public QObject
{
    Q_OBJECT

public:
    static windowManager& instance();

    void showStartWindow();
    void showLoginWindow();
    void showMainWindow();
    void showTransactionWindow();
    void showWithdrawWindow();
    void showWithdrawOtherSumWindow();
    void showWithdrawEndWindow();
    void showBalanceWindow();

private:
    windowManager() = default;
    ~windowManager() = default;

};

#endif // WINDOWMANAGER_H
