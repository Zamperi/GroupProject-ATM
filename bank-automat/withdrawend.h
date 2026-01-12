//Tämä luokka on rahan noston päättymistä varten

#ifndef WITHDRAWEND_H
#define WITHDRAWEND_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class withdrawEnd;
}

class withdrawEnd : public QDialog
{
    Q_OBJECT

public:
    explicit withdrawEnd(QWidget *parent = nullptr);
    ~withdrawEnd();

private:
    Ui::withdrawEnd *ui;
    QTimer *inactivityTimer;
    void onTimeout();
    void resetTimer();
    void updateBalance(QString);
    void withdrawFailed(QString);
    void setupGraphicsView();
};

#endif // WITHDRAWEND_H
