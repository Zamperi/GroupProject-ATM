//Tämä luokka on tietokantapyyntöjen käsittelyä varten

#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QString>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtNetwork>
#include <QJsonDocument>

class dataManager : public QObject
{
    Q_OBJECT

public:
    static dataManager& instance();
    QString getToken() const;
    void setToken(QString inputToken);
    void handleLoginReply(QNetworkReply *reply);
    void attemptLogin(const QString&, const QString&);
    void fetchBalance();

signals:
    void loginFailed(QString);
    void loginSucceeded(QString);
    void transactionsFetched(QJsonArray);
    void withdrawSucceeded(const QString& newBalance);
    void withdrawFailed(const QString& errorMessage);
    void fetchedBalance(float, float, int);

public slots:
    void fetchTransactions(const QString &token);
    void handleTransactionReply(QNetworkReply *reply);
    void withdraw (double);
    void handleWithdrawReply(QNetworkReply *reply);

private:
    QString token;
    dataManager();
    ~dataManager();
    QByteArray response_data;
    QNetworkAccessManager *networkManager;
};

#endif // DATAMANAGER_H
