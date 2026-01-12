//Tämä luokka on pääikkunan hallintaa varten

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_btnLogOut_clicked();
    void on_btnShowBalance_clicked();
    void on_btnWithdraw_clicked();
    void on_btnTransactions_clicked();

private:
    Ui::MainWindow *ui;
    void setupGraphicsView();
    QTimer *inactivityTimer;
    void resetInactivityTimer();
    void onInactivityTimeout();
};
#endif // MAINWINDOW_H
