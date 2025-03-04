
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "types.h"

#define AD7606_REGISTER_STATUS                   0x01
#define AD7606_REGISTER_CONFIG                   0x02
#define AD7606_REGISTER_RANGE_CH1_2              0x03
#define AD7606_REGISTER_RANGE_CH3_4              0x04
#define AD7606_REGISTER_RANGE_CH5_6              0x05
#define AD7606_REGISTER_RANGE_CH7_8              0x06
#define AD7606_REGISTER_BANDWIDTH                0x07
#define AD7606_REGISTER_OVERSAMPLING             0x08
#define AD7606_REGISTER_GAIN_CH(ch)             (0x09 + (ch))
#define AD7606_REGISTER_OFFSET_CH(ch)           (0x11 + (ch))
#define AD7606_REGISTER_PHASE_CH(ch)            (0x19 + (ch))
#define AD7606_REGISTER_DIGITAL_DIAG_ENABLE      0x21
#define AD7606_REGISTER_DIGITAL_DIAG_ERR         0x22
#define AD7606_REGISTER_OPEN_DETECT_ENABLE       0x23
#define AD7606_REGISTER_OPEN_DETECTED            0x24
#define AD7606_REGISTER_AIN_OV_UV_DIAG_ENABLE    0x25
#define AD7606_REGISTER_AIN_OV_DIAG_ERROR        0x26
#define AD7606_REGISTER_AIN_UV_DIAG_ERROR        0x27
#define AD7606_REGISTER_DIAGNOSTIC_MUX_CH1_2     0x28
#define AD7606_REGISTER_DIAGNOSTIC_MUX_CH3_4     0x29
#define AD7606_REGISTER_DIAGNOSTIC_MUX_CH5_6     0x2A
#define AD7606_REGISTER_DIAGNOSTIC_MUX_CH7_8     0x2B
#define AD7606_REGISTER_OPEN_DETECT_QUEUE        0x2C
#define AD7606_REGISTER_CLK_FS_COUNTER           0x2D
#define AD7606_REGISTER_CLK_OS_COUNTER           0x2E
#define AD7606_REGISTER_ID                       0x2F

#define AD7606_WEN(x)    (x << 7)
#define AD7606_RW(x)     (x << 6)

struct ad7606_device {
    int fd;
    int frequency;
};

int ad7606_init(struct ad7606_device* device, char* spi_device, int frequency);
int ad7606_transfer_frame(struct ad7606_device* device, u8 tx[], u8 rx[], u32 length);
int ad7606_read_register(struct ad7606_device* device, u8 address, u8* data);
int ad7606_write_register(struct ad7606_device* device, u8 address, u8 data);
float ad7606_convert_raw_value(u32 raw);