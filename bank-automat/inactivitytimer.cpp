#include "inactivitytimer.h"

//Tehdään luokasta singleton
InactivityTimer& InactivityTimer::instance()
{
    static InactivityTimer instance;
    return instance;
}

// yhdistetään ajastimen timeout-signaali luokan timeout-signaaliin
InactivityTimer::InactivityTimer(QObject *parent) : QObject(parent) {
    connect(&timer, &QTimer::timeout, this, &InactivityTimer::timeout);
}

// Ajastimen käynnistys
void InactivityTimer::startTimer(int timeoutMs) {
    timer.setInterval(timeoutMs);
    timer.start();
}

// Ajastimen nollaus ja uudelleenkäynnistys
void InactivityTimer::resetTimer() {
    timer.start();  // Käynnistää ajastimen uudelleen nykyisellä aikavälillä
}

// Ajastimen pysäytys
void InactivityTimer::stopTimer() {
    timer.stop();
}
