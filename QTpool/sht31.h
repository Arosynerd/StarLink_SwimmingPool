#ifndef SHT31_H
#define SHT31_H

#include <QObject>
extern "C" {
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include <time.h>
}

class SHT31 : public QObject
{
    Q_OBJECT
public:
    explicit SHT31(QObject *parent = nullptr);

    int sht31_init();                          // 初始化I2C通信
    int sht31_send_command(uint16_t command); // 发送命令到SHT31
    int sht31_read_data(uint8_t *data, size_t len); // 从SHT31读取数据
    int sht31_read_temp_humidity(float *temperature, float *humidity); // 读取温湿度
    void sht31_close();                       // 关闭I2C通信

private:
    int i2c_fd; // I2C文件描述符
    const char *i2c_bus = "/dev/i2c-2"; // I2C总线路径
    const uint8_t sht31_addr = 0x44;    // SHT31默认I2C地址
};

#endif // SHT31_H