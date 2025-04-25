#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QList>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //创建蜂鸣器对象
    Beep = new beep(this);
    //
    Sht31 = new SHT31(this);
    ui->setupUi(this);
    //设置标题
    this->setWindowTitle("星闪泳池监控系统");
    //设置窗体背景图
    //允许设置背景图
      this->setAutoFillBackground(true);
   //    //创建图片控件,选择图片，并且设置大小

       QPixmap pix1 = QPixmap(":/image/swim.png").scaled(this->size());

   //    //将图片设置为背景
       QPalette palette;
      palette.setBrush(QPalette::Background,QBrush(pix1));
   
      this->setPalette(palette);
      //温湿度
        //设置文字大小
      ui->humi_label->setFont(QFont("Arial", 20, QFont::Bold));
      ui->temp_label->setFont(QFont("Arial", 20, QFont::Bold));
      ui->humivalue_label->setFont(QFont("Arial", 20, QFont::Bold));
      ui->tempvalue_label->setFont(QFont("Arial", 20, QFont::Bold));
    //this->setStyleSheet("background-image: url(file:///f:/Users/17444/Documents/qt/QTpool/swim.png);");
    timerId = startTimer(1000); // 启动定时器，每隔1000毫秒触发一次定时器事件
    // 获取所有label控件并设置内容为“null”
    QList<QLabel *> labels = this->findChildren<QLabel *>();
    for (QLabel *label : labels) {
        if (label->objectName().startsWith("label")) { // 检查标签名是否以"label"开头
            label->setText("swimmer");
            label->setFont(QFont("Arial", 10, QFont::Bold));
            label->setStyleSheet("color: red;");
        }
    }

}
void MainWindow::timerEvent(QTimerEvent *event) {
    static int currentIndex = 0; // 用于跟踪当前移动的label索引
    QList<QLabel *> labels = this->findChildren<QLabel *>();
    labels.erase(std::remove_if(labels.begin(), labels.end(), [](QLabel *label) {
        return !label->objectName().startsWith("label");
    }), labels.end());

    if (labels.isEmpty()) return;

    // 移动当前索引的label
    QLabel *currentLabel = labels[currentIndex];
    int x = currentLabel->x() + 10; // 每次向右移动10个像素
    int y = currentLabel->y();      // 保持y坐标不变
    currentLabel->move(x, y);

    // 更新索引，循环移动
    currentIndex = (currentIndex + 1) % labels.size();


    //test get humi and temp
    static float humi = 15.00,temp = 36.00;
    if(Sht31->sht31_read_temp_humidity(&temp,&humi) == 0){

    }
    ui->tempvalue_label->setText(QString::number(temp,'f',2));
    ui->humivalue_label->setText(QString::number(humi,'f',2));
}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_turnon_clicked()
{
    Beep->Beep_On();
}

void MainWindow::on_turnoff_clicked()
{
    Beep->Beep_Off();
}
