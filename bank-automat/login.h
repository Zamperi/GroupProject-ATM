//Tämä luokka on kirjautumisen hallintaa varten

#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <qlineedit.h>
#include <QTimer>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    void loginSuccess();
    void loginFailure(QString);

private slots:
    void on_buttonOK_clicked();
    void onNumberButtonClicked();
    void onSetCardNumberActive();
    void onSetPinActive();
    void on_buttonClear_clicked();

private:
    Ui::Login *ui;
    QNetworkAccessManager *loginManager;
    QNetworkReply *reply;
    QByteArray response_data;
    QLineEdit *activeLineEdit;
    QTimer *inactivityTimer;
    void resetInactivityTimer();
    void onInactivityTimeout();
    void setupGraphicsView();
};

#endif // LOGIN_H
