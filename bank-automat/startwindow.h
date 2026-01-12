//Tämä luokka on kirjautumisikkunaa varten

#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QDialog>
#include "ui_startwindow.h"

namespace Ui {
class startWindow;
}

class startWindow : public QDialog
{
    Q_OBJECT

public:
    explicit startWindow(QWidget *parent = nullptr);
    ~startWindow();

private slots:
    void on_btnStart_clicked();

private:
    Ui::startWindow *ui;
    void setupGraphicsView();
};

#endif // STARTWINDOW_H
