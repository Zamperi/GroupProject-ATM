#ifndef BALANCEWINDOW_H
#define BALANCEWINDOW_H

#include <QDialog>
#include "datamanager.h"
#include "windowmanager.h"

namespace Ui {
class BalanceWindow;
}

class BalanceWindow : public QDialog
{
    Q_OBJECT

public:
    explicit BalanceWindow(QWidget *parent = nullptr);
    void connectDataManager();
    ~BalanceWindow();

public slots:
    void setLabels(float, float, int);

private slots:
    void on_btnReturn_clicked();

    void on_btnLogout_clicked();

private:
    Ui::BalanceWindow *ui;
    void setupGraphicsView();
    QTimer *inactivityTimer;
    void resetInactivityTimer();
    void onInactivityTimeout();
};

#endif // BALANCEWINDOW_H
