#ifndef __LIBMYDEV_GPIO_H__
#define __LIBMYDEV_GPIO_H__

#include "mydev_gpio.h"

int MYDEV_GPIO_SetDirection(unsigned char gpio_num, unsigned char gpio_dir);
int MYDEV_GPIO_SetValue(unsigned char gpio_num, unsigned char gpio_val);
int MYDEV_GPIO_GetValue(unsigned char gpio_num, unsigned char *gpio_val);

#endif /*__LIBMYDEV_GPIO_H__*/
