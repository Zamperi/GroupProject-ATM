//Tämä luokka on rahannostoa varten

#ifndef WITHDRAWWINDOW_H
#define WITHDRAWWINDOW_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>

namespace Ui {
class withdrawWindow;
}

class withdrawWindow : public QDialog
{
    Q_OBJECT

public:
    explicit withdrawWindow(QWidget *parent = nullptr);
    ~withdrawWindow();

private slots:
    void on_btnReturn_clicked();
    void on_btnLogout_clicked();
    void on_btn20_clicked();
    void on_btn40_clicked();
    void on_btn50_clicked();
    void on_btn100_clicked();
    void on_btnOtherSum_clicked();

private:
    Ui::withdrawWindow *ui;
    void setupGraphicsView();
    QTimer *inactivityTimer;
    void resetInactivityTimer();
    void onInactivityTimeout();
};

#endif // WITHDRAWWINDOW_H
