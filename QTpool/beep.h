#ifndef BEEP_H
#define BEEP_H

#include <QObject>
extern "C"{

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
}



class beep : public QObject
{
    Q_OBJECT
public:
    explicit beep(QObject *parent = nullptr);

    int fd, retvalue;
    char *filename;
    unsigned char databuf[1];
    void Beep_On();
    void Beep_Off();


signals:

public slots:
};

#endif // BEEP_H
