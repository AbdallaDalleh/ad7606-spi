
#include "ad7606.h"

int ad7606_init(struct ad7606_device* device, char* spi_device, int frequency)
{
    int status;
    int fd_spi;
    uint8_t mode = 1;
    uint8_t bits = 8;

    fd_spi = open(spi_device, O_RDWR);
    if (fd_spi == -1) {
        perror("open");
        return -1;
    }

    status = ioctl(fd_spi, SPI_IOC_WR_MODE, &mode);
    if (status != 0) {
        perror("ioctl: mode");
        return -2;
    }

    status = ioctl(fd_spi, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (status != 0) {
        perror("ioctl: bits");
        return -2;
    }

    status = ioctl(fd_spi, SPI_IOC_WR_MAX_SPEED_HZ, &frequency);
    if (status != 0) {
        perror("ioctl: speed");
        return -2;
    }

    device->fd = fd_spi;
    device->frequency = frequency;
    return 0;
}

int ad7606_transfer_frame(struct ad7606_device* device, u8 tx[], u8 rx[], u32 length)
{
    struct spi_ioc_transfer spi_transfer = {
        .tx_buf = (unsigned long) tx,
        .rx_buf = (unsigned long) rx,
        .len = length,
        .speed_hz = device->frequency,
        .delay_usecs = 0,
        .bits_per_word = 8,
        .cs_change = 1
    };

    return ioctl(device->fd, SPI_IOC_MESSAGE(1), &spi_transfer);
}

int ad7606_read_register(struct ad7606_device* device, u8 address, u8* data)
{
    int status;
    u8 tx[2];
    u8 rx[2];

    tx[0] = AD7606_WEN(0) | AD7606_RW(1) | address;
    tx[1] = 0;
    status = ad7606_transfer_frame(device, tx, NULL, 2);
    if (status == -1) {
        return status;
    }

    status = ad7606_transfer_frame(device, NULL, rx, 2);
    if (status == -1) {
        return status;
    }

    *data = rx[1];
    return status;
}

int ad7606_write_register(struct ad7606_device* device, u8 address, u8 data)
{
    u8 tx[2];

    tx[0] = AD7606_WEN(0) | AD7606_RW(0) | address;
    tx[1] = data;
    return ad7606_transfer_frame(device, tx, NULL, 2);
}

float ad7606_convert_raw_value(u32 raw)
{
    float value;

    if (raw & 0x20000) {
        raw ^= 0x3FFFF;
        raw += 0x1;
        value = raw * -0.0000762939453125;
    }
    else {
        value = raw * 0.0000762939453125;
    }

    return value;
}