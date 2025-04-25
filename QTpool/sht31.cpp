#include "sht31.h"
#include <QMessageBox>

// SHT31命令定义
#define CMD_MEASURE_HIGHREP 0x2400
#define CMD_MEASURE_MEDREP  0x240B
#define CMD_MEASURE_LOWREP  0x2416
#define CMD_READSTATUS      0xF32D
#define CMD_CLEARSTATUS     0x3041
#define CMD_SOFTRESET       0x30A2
#define CMD_HEATEREN        0x306D
#define CMD_HEATERDIS       0x3066

SHT31::SHT31(QObject *parent) : QObject(parent), i2c_fd(-1)
{
    // 初始化I2C总线和SHT31
    if (sht31_init() < 0) {
        QMessageBox::critical(nullptr, "错误", "初始化SHT31失败");
    }
    QMessageBox::critical(nullptr, "ttest", "test");
}

int SHT31::sht31_init()
{
    if ((i2c_fd = ::open(i2c_bus, O_RDWR)) < 0) {
        QMessageBox::critical(nullptr, "错误", "无法打开I2C总线");
        return -1;
    }

    if (::ioctl(i2c_fd, I2C_SLAVE, sht31_addr) < 0) {
        QMessageBox::critical(nullptr, "错误", "无法设置I2C从设备地址");
        ::close(i2c_fd);
        return -1;
    }

    // 发送软复位命令
    sht31_send_command(CMD_SOFTRESET);
    ::usleep(10000);  // 等待复位完成

    return 0;
}

int SHT31::sht31_send_command(uint16_t command)
{
    uint8_t buf[2];
    buf[0] = command >> 8;    // 高字节
    buf[1] = command & 0xFF;  // 低字节

    if (::write(i2c_fd, buf, 2) != 2) {
        QMessageBox::critical(nullptr, "错误", "发送命令到SHT31失败");
        return -1;
    }
    return 0;
}

int SHT31::sht31_read_data(uint8_t *data, size_t len)
{
    if (::read(i2c_fd, data, len) != len) {
        QMessageBox::critical(nullptr, "错误", "从SHT31读取数据失败");
        return -1;
    }

    return 0;
}

int SHT31::sht31_read_temp_humidity(float *temperature, float *humidity)
{
    uint8_t data[6];
    
    // 发送高精度测量命令
    if (sht31_send_command(CMD_MEASURE_HIGHREP) < 0) {
        return -1;
    }
    
    // 等待测量完成(高精度模式最大需要15ms)
    ::usleep(20000);  // 等待20ms确保完成
    
    // 读取6字节数据
    if (sht31_read_data(data, 6) < 0) {
        return -1;
    }
    
    // 转换温度数据
    uint16_t temp_raw = (data[0] << 8) | data[1];
    *temperature = -45 + 175 * (temp_raw / 65535.0);
    
    // 转换湿度数据
    uint16_t hum_raw = (data[3] << 8) | data[4];
    *humidity = 100 * (hum_raw / 65535.0);
    
    return 0;
}

void SHT31::sht31_close()
{
    if (i2c_fd >= 0) {
        ::close(i2c_fd);
        i2c_fd = -1;
    }
}
