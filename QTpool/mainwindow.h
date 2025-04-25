#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "beep.h"
#include "sht31.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
enum SwimState {
    NormalSwim,
    Struggling,
    Stationary,
    Sinking,        //水位较低
    HighHeartRate   //高心率
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void timerEvent(QTimerEvent *event);
    ~MainWindow();

    beep *Beep;
    SHT31 *Sht31;

private slots:
    void on_turnon_clicked();

    void on_turnoff_clicked();

private:
    Ui::MainWindow *ui;
    int timerId;
};
#endif // MAINWINDOW_H
