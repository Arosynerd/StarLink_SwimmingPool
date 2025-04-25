#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
    int timerId;
};
#endif // MAINWINDOW_H
