#ifndef __MYDEV_GPIO_H__
#define __MYDEV_GPIO_H__

#define MYDEV_IOCTL_MAGIC       254 // arbitrary number

#define MYDEV_IOCTL_GPIO_DIR    _IOW(MYDEV_IOCTL_MAGIC, 0x0, unsigned char*)
#define MYDEV_IOCTL_GPIO_SET    _IOW(MYDEV_IOCTL_MAGIC, 0x1, unsigned char*)
#define MYDEV_IOCTL_GPIO_GET    _IOR(MYDEV_IOCTL_MAGIC, 0x2, unsigned char*)

#define MYDEV_IOCTL_MAX_NR	3

#endif /*__MYDEV_GPIO_H__*/

