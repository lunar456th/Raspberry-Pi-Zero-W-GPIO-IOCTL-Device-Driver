#include "libmydev_gpio.h"
#include "mydev_gpio.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/syslog.h>

const static char *dev_name = "/dev/mydev0";
unsigned char bInitialized = 0;
int mydev_fd = 0;

static int _MYDEV_GPIO_Initialize(void)
{
    if ((mydev_fd = open(dev_name, O_RDWR)) < 0)
    {   
        syslog(LOG_INFO, "[%s:%d] open failed", __func__, __LINE__);
        return -1; 
    }   
    bInitialized = 1;
    syslog(LOG_INFO, "[%s:%d] open success", __func__, __LINE__);
    return 0;
}

int MYDEV_GPIO_SetDirection(unsigned char gpio_num, unsigned char gpio_dir)
{
    int ret = 0;
	unsigned char gpio_info[2] = { 0, };

    if (!bInitialized && _MYDEV_GPIO_Initialize() < 0)
	{
		syslog(LOG_INFO, "[%s:%d] _MYDEV_GPIO_SetDirection failed", __func__, __LINE__);
		return -1;
	}

	gpio_info[0] = gpio_num;
	gpio_info[1] = gpio_dir;

    syslog(LOG_INFO, "[%s:%d] ioctl MYDEV_IOCTL_GPIO_DIR (gpio_num = %d, gpio_dir = %d)", __func__, __LINE__, gpio_info[0], gpio_info[1]);

    if ((ret = ioctl(mydev_fd, MYDEV_IOCTL_GPIO_DIR, gpio_info)) < 0)
    {
        syslog(LOG_INFO, "[%s:%d] ioctl MYDEV_IOCTL_GPIO_DIR failed", __func__, __LINE__);
        return -1;
    }

    syslog(LOG_INFO, "[%s:%d] ioctl MYDEV_IOCTL_GPIO_DIR success", __func__, __LINE__);
    return 0;
}

int MYDEV_GPIO_SetValue(unsigned char gpio_num, unsigned char gpio_val)
{
    int ret = 0;
	unsigned char gpio_info[2] = { 0, };

    if (!bInitialized && _MYDEV_GPIO_Initialize() < 0)
	{
		syslog(LOG_INFO, "[%s:%d] _MYDEV_GPIO_SetDirection failed", __func__, __LINE__);
		return -1;
	}

	gpio_info[0] = gpio_num;
	gpio_info[1] = gpio_val;

    syslog(LOG_INFO, "[%s:%d] ioctl MYDEV_IOCTL_GPIO_SET (gpio_num = %d, gpio_val = %d)", __func__, __LINE__, gpio_info[0], gpio_info[1]);

    if ((ret = ioctl(mydev_fd, MYDEV_IOCTL_GPIO_SET, gpio_info)) < 0)
    {
        syslog(LOG_INFO, "[%s:%d] ioctl MYDEV_IOCTL_GPIO_SET failed", __func__, __LINE__);
        return -1;
    }

    syslog(LOG_INFO, "[%s:%d] ioctl MYDEV_IOCTL_GPIO_SET success", __func__, __LINE__);
    return 0;
}

int MYDEV_GPIO_GetValue(unsigned char gpio_num, unsigned char *gpio_val)
{
    int ret = 0;
	unsigned char gpio_info[2] = { 0, };

    if (!bInitialized && _MYDEV_GPIO_Initialize() < 0)
	{
		syslog(LOG_INFO, "[%s:%d] _MYDEV_GPIO_SetDirection failed", __func__, __LINE__);
		return -1;
	}

	gpio_info[0] = gpio_num;
	gpio_info[1] = 0; // not used

    syslog(LOG_INFO, "[%s:%d] ioctl MYDEV_IOCTL_GPIO_GET (gpio_num = %d)", __func__, __LINE__, gpio_info[0]);

    if ((ret = ioctl(mydev_fd, MYDEV_IOCTL_GPIO_GET, gpio_info)) < 0)
    {
        syslog(LOG_INFO, "[%s:%d] ioctl MYDEV_IOCTL_GPIO_GET failed", __func__, __LINE__);
        return -1;
    }

    *gpio_val = gpio_info[0];

    syslog(LOG_INFO, "[%s:%d] ioctl MYDEV_IOCTL_GPIO_GET (value = %d)", __func__, __LINE__, *gpio_val);
    return 0;
}

