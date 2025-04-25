#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

#define SHT31_ADDR 0x44  // SHT31默认I2C地址
#define I2C_BUS "/dev/i2c-2"  // 根据实际使用的I2C总线修改

// SHT31命令定义
#define CMD_MEASURE_HIGHREP 0x2400
#define CMD_MEASURE_MEDREP  0x240B
#define CMD_MEASURE_LOWREP  0x2416
#define CMD_READSTATUS      0xF32D
#define CMD_CLEARSTATUS     0x3041
#define CMD_SOFTRESET       0x30A2
#define CMD_HEATEREN        0x306D
#define CMD_HEATERDIS       0x3066

// I2C文件描述符
static int i2c_fd = -1;

// 初始化I2C通信
int sht31_init() {
    if ((i2c_fd = open(I2C_BUS, O_RDWR)) < 0) {
        perror("Failed to open I2C bus");
        return -1;
    }

    if (ioctl(i2c_fd, I2C_SLAVE, SHT31_ADDR) < 0) {
        perror("Failed to set I2C slave address");
        close(i2c_fd);
        return -1;
    }

    return 0;
}

// 发送命令到SHT31
int sht31_send_command(uint16_t command) {
    uint8_t buf[2];
    buf[0] = command >> 8;    // 高字节
    buf[1] = command & 0xFF;  // 低字节

    if (write(i2c_fd, buf, 2) != 2) {
        perror("Failed to send command to SHT31");
        return -1;
    }
    return 0;
}

// 从SHT31读取数据
int sht31_read_data(uint8_t *data, size_t len) {
    if (read(i2c_fd, data, len) != len) {
        perror("Failed to read data from SHT31");
        return -1;
    }
    return 0;
}

// 读取温度和湿度
int sht31_read_temp_humidity(float *temperature, float *humidity) {
    uint8_t data[6];
    
    // 发送高精度测量命令
    if (sht31_send_command(CMD_MEASURE_HIGHREP) < 0) {
        return -1;
    }
    
    // 等待测量完成(高精度模式最大需要15ms)
    usleep(20000);  // 等待20ms确保完成
    
    // 读取6字节数据
    if (sht31_read_data(data, 6) < 0) {
        return -1;
    }
    
    // 验证CRC (可选)
    // 这里省略CRC校验代码
    
    // 转换温度数据
    uint16_t temp_raw = (data[0] << 8) | data[1];
    *temperature = -45 + 175 * (temp_raw / 65535.0);
    
    // 转换湿度数据
    uint16_t hum_raw = (data[3] << 8) | data[4];
    *humidity = 100 * (hum_raw / 65535.0);
    
    return 0;
}

// 关闭I2C
void sht31_close() {
    if (i2c_fd >= 0) {
        close(i2c_fd);
        i2c_fd = -1;
    }
}

int main() {
    float temperature, humidity;
    
    if (sht31_init() < 0) {
        fprintf(stderr, "SHT31 initialization failed\n");
        return EXIT_FAILURE;
    }
    
    // 可选: 发送软复位命令
    sht31_send_command(CMD_SOFTRESET);
    usleep(10000);  // 等待复位完成
    
    while (1) {
        if (sht31_read_temp_humidity(&temperature, &humidity) == 0) {
            printf("Temperature: %.2f°C, Humidity: %.2f%%\n", temperature, humidity);
        } else {
            fprintf(stderr, "Failed to read from SHT31\n");
        }
        
        sleep(1);  // 每秒读取一次
    }
    
    sht31_close();
    return EXIT_SUCCESS;
}