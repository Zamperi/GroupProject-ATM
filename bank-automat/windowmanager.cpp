#include "windowmanager.h"

windowManager& windowManager::instance()
{
    static windowManager instance; //Singleton
    return instance;
}

void windowManager::showStartWindow()
{
    startWindow *objStart = new startWindow(nullptr);
    objStart->setAttribute(Qt::WA_DeleteOnClose); // Automaattinen tuhoaminen
    objStart->show();
}

void windowManager::showLoginWindow()
{
    Login *objLogin = new Login(nullptr);
    objLogin->setAttribute(Qt::WA_DeleteOnClose);
    objLogin->show();
}

void windowManager::showMainWindow()
{
    MainWindow *objMainWindow = new MainWindow(nullptr);
    objMainWindow->setAttribute(Qt::WA_DeleteOnClose);
    objMainWindow->show();
}

void windowManager::showTransactionWindow()
{
    transactionWindow *objTransactionWindow = new transactionWindow(nullptr);
    objTransactionWindow->setAttribute(Qt::WA_DeleteOnClose);
    objTransactionWindow->show();
}

void windowManager::showWithdrawWindow()
{
    withdrawWindow *objWithdrawWindow = new withdrawWindow(nullptr);
    objWithdrawWindow->setAttribute(Qt::WA_DeleteOnClose);
    objWithdrawWindow->show();

}

void windowManager::showWithdrawOtherSumWindow()
{
    withdrawOtherSum *objWithdrawOtherSumWindow = new withdrawOtherSum(nullptr);
    objWithdrawOtherSumWindow->setAttribute(Qt::WA_DeleteOnClose);
    objWithdrawOtherSumWindow->show();
}

void windowManager::showWithdrawEndWindow()
{
    withdrawEnd *objWithdrawEndWindow = new withdrawEnd(nullptr);
    objWithdrawEndWindow->setAttribute(Qt::WA_DeleteOnClose);
    objWithdrawEndWindow->show();
}

void windowManager::showBalanceWindow()
{
    BalanceWindow *objBalanceWindow = new BalanceWindow(nullptr);
    objBalanceWindow->setAttribute(Qt::WA_DeleteOnClose);
    objBalanceWindow->show();
}
