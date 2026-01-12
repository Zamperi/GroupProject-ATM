//Tämä luokka on globaali ajastin
//käyttäjän epäaktiivisuutta ja ikkunoiden sulkemista varten

#ifndef INACTIVITYTIMER_H
#define INACTIVITYTIMER_H

#include <QObject>
#include <QTimer>
#include <QDebug>

class InactivityTimer : public QObject
{
    Q_OBJECT

public:
    static InactivityTimer& instance();
    void startTimer(int timeOutMs);
    void resetTimer();
    void stopTimer();

signals:
    void timeout();

private:
    QTimer timer;

    explicit InactivityTimer(QObject *parent = nullptr);
    ~InactivityTimer() = default;

    // Estetään kopiointi ja siirto
    InactivityTimer(const InactivityTimer&) = delete;
    InactivityTimer& operator=(const InactivityTimer&) = delete;
    InactivityTimer(InactivityTimer&&) = delete;
    InactivityTimer& operator=(InactivityTimer&&) = delete;
};

#endif // INACTIVITYTIMER_H
