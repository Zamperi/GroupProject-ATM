//Tämä luokka on aktiivisen tekstikentän valintaa varten.

#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QLineEdit>
#include <QDebug>

class CustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CustomLineEdit(QWidget *parent = nullptr) : QLineEdit(parent) {}

protected:
    void focusInEvent(QFocusEvent *event) override {
        QLineEdit::focusInEvent(event);
        emit lineEditFocused(); // Lähetetään oma signaali
    }

signals:
    void lineEditFocused();
};

#endif // CUSTOMLINEEDIT_H
