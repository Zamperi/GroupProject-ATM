#include "datamanager.h"
#include "environment.h"

dataManager& dataManager::instance()
{
    static dataManager instance;
    return instance;
}

dataManager::dataManager()
{
    // Alustetaan networkManager
    networkManager = new QNetworkAccessManager(this);
}

dataManager::~dataManager()
{
    delete networkManager;
}

//Tokenin hallinta
void dataManager::setToken(QString inputToken)
{
    token = inputToken;
    qDebug()<<"Token asetettu:"<<inputToken;
}

QString dataManager::getToken() const
{
    return token;
}

//Loginin hallinta
void dataManager::attemptLogin(const QString& inputCardNum, const QString& inputPin)
{
    QJsonObject jsonObj;
    jsonObj.insert("cardnum", inputCardNum);
    jsonObj.insert("pin", inputPin);

    QNetworkRequest request;
    request.setUrl(QUrl(Environment::base_url() + "/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(jsonObj).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleLoginReply(reply);
    });
}

// Saldon hakeminen
void dataManager::fetchBalance()
{
    std::unique_ptr<QNetworkRequestFactory> requestFactory (new QNetworkRequestFactory);
    requestFactory->setBaseUrl(QUrl(Environment::base_url()));
    QByteArray tokenByteArray = token.toUtf8();
    requestFactory->setBearerToken(tokenByteArray);

    QNetworkReply *reply = networkManager->get(requestFactory->createRequest("/account"));

    connect(networkManager, &QNetworkAccessManager::finished,
            this, [=] {
        if(reply->error()) {
            qDebug() << reply->errorString();
        } else {
            QByteArray replyText = reply->readAll();
            qDebug() << replyText;

            QJsonDocument replyDoc = QJsonDocument::fromJson(replyText);
            QJsonObject replyObj = replyDoc.object();

            QJsonObject accountObj = replyObj.value("account").toObject();
            float balance = accountObj.value("balance").toString().toFloat();
            qDebug() << accountObj.value("balance");
            qDebug() << balance;
            float creditLimit = accountObj.value("credit_limit").toString().toFloat();
            qDebug() << creditLimit;
            int typeCredit = accountObj.value("type_credit").toInt();
            qDebug() << typeCredit;

            qDebug() << balance;
            emit fetchedBalance(balance, creditLimit, typeCredit);
        }
    });
}

void dataManager::handleLoginReply(QNetworkReply *reply)
{

    response_data = reply->readAll();
    qDebug() <<"Yritetään kirjautua"<< response_data;

    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);
    QJsonObject jsonObject = jsonResponse.object();

    // Tarkista, sisältääkö vastaus virheviestin
    if (jsonObject.contains("error")) {
        emit loginFailed(jsonObject["error"].toString());
        qDebug()<<"Epäonnistui";
    }
    // Tarkista, sisältääkö vastaus onnistumisviestin ja tallennetaan token
    else if (jsonObject.contains("message")) {
        setToken(jsonObject["message"].toString());
        emit loginSucceeded(token);
    }

    // Siivoa resurssit
    reply->deleteLater();
}

//Tilitapahtumien näyttö
void dataManager::fetchTransactions(const QString& token)
{
    QNetworkRequest request;
    request.setUrl(QUrl(QString("%1/transaction/latestTransactions").arg(Environment::base_url())));
    request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    qDebug()<<"Tilitapahtumat haetaan!"<<token;
    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleTransactionReply(reply);
    });
}

void dataManager::handleTransactionReply(QNetworkReply *reply)
{
    qDebug()<<"handling transaction reply";
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);

        if (jsonDoc.isArray()) {
            QJsonArray transactionsArray = jsonDoc.array();

            emit transactionsFetched(transactionsArray);
        }
    } else {
        qDebug() << "Error fetching transactions:" << reply->errorString();
    }
    reply->deleteLater();
}

//Rahan nostaminen
void dataManager::withdraw(double amount)
{
    qDebug()<<"Nostetaan: "<<amount;
    QJsonObject jsonObj;
    jsonObj.insert("amount", amount);

    QNetworkRequest request;
    request.setUrl(QUrl(Environment::base_url() + "/withdraw"));
    request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //networkManager->post(request, QJsonDocument(jsonObj).toJson());
    QNetworkReply *reply = networkManager->post(request, QJsonDocument(jsonObj).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleWithdrawReply(reply);
    });
}

void dataManager::handleWithdrawReply(QNetworkReply *reply)
{
    qDebug()<<"Handling withdraw reply.";
    QByteArray response = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
    QJsonObject jsonObj = jsonDoc.object();

    if (reply->error() == QNetworkReply::NoError) {
        if (jsonDoc.isObject()) {
            if (jsonObj.contains("newBalance") && jsonObj["newBalance"].isDouble()) {
                double newBalance = jsonObj["newBalance"].toDouble();
                emit withdrawSucceeded(QString::number(newBalance, 'f', 2));
            }
        }
    } else {
        qDebug() << "Error withdrawing money:" << reply->errorString();
        emit withdrawFailed(jsonObj["error"].toString());
    }
    reply->deleteLater();
}
