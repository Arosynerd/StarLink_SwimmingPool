#include "beep.h"
#include <QMessageBox>
beep::beep(QObject *parent) : QObject(parent)
{
    fd = ::open("/sys/class/gpio/export", O_WRONLY);
    if(fd < 0){
        QMessageBox::critical(nullptr, "Error", "Failed to open /sys/class/gpio/export");
    }
    else{
        QMessageBox::information(nullptr, "OK", "succeeded to open /sys/class/gpio/export");
    }
    retvalue = ::write(fd, "23", strlen("23"));
    if(retvalue < 0){
       QMessageBox::critical(nullptr, "Error", "Failed to open /sys/class/gpio/direction");
    }
    else{
        QMessageBox::information(nullptr, "OK", "succeeded to open /sys/class/gpio/direction");
    }

    close(fd);
    fd = ::open("/sys/class/gpio/gpio23/direction", O_WRONLY);

    retvalue = ::write(fd, "out", 3);
    close(fd);

}


void beep::Beep_On()
{
   // databuf[0] = 1; /* 要执行的操作：打开或关闭 */
    fd = ::open("/sys/class/gpio/gpio23/value",O_WRONLY);
    retvalue = ::write(fd, "1", 1);
    if(retvalue < 0){
        QMessageBox::critical(nullptr, "Error", "Failed to open /sys/class/gpio/export");
    }
    else{
        QMessageBox::information(nullptr, "OK", "succeeded to open /sys/class/gpio/export");
    }
    close(fd);
    /* 向/dev/beep 文件写入数据 */
//    retvalue = ::write(fd, databuf, sizeof(databuf));
//    if(retvalue < 0){
//        ::close(fd);
//        return;
//    }

}

void beep::Beep_Off()
{
    fd = ::open("/sys/class/gpio/gpio23/value",O_WRONLY);
    retvalue = ::write(fd, "0", 1);
    if(retvalue < 0){
        QMessageBox::critical(nullptr, "Error", "Failed to open /sys/class/gpio/export");
    }
    else{
        QMessageBox::information(nullptr, "OK", "succeeded to open /sys/class/gpio/export");
    }
    close(fd);
//    databuf[0] = 0;
//    retvalue = ::write(fd, databuf, sizeof(databuf));
//    if(retvalue < 0){
//         ::close(fd);
//         return ;
//    }
}

