#include "mydev_gpio.h"

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <asm/current.h>
#include <asm/io.h>
#include <asm/ioctl.h>

// GPIO Address Macro
#if 1
#define BCM_IO_BASE (0x20000000) // for BCM2708 (Pi 1, Zero, Zero W)
#else
#define BCM_IO_BASE (0x3F000000) // for BCM2709 (Pi 2, 3, 3+)
#endif

#define GPIO_BASE   (BCM_IO_BASE + 0x200000)
#define GPIO_SIZE   (256)

// GPIO Setting Macro
#define GPIO_IN(g)  (*(gpio + ((g) / 10)) &= ~(7 << (((g) % 10) * 3)))
#define GPIO_OUT(g) (*(gpio + ((g) / 10)) |= (1 << (((g) % 10) * 3)))

#define GPIO_SET(g) (*(gpio + 7) = 1 << g)
#define GPIO_CLR(g) (*(gpio + 10) = 1 << g)
#define GPIO_GET(g) (*(gpio + 13) & (1 << g))

static const unsigned int	MINOR_BASE = 0;
static const unsigned int	MINOR_NUM  = 1;
static unsigned int			my_major;
static struct cdev			my_cdev;
static struct class			*my_class = NULL;
static dev_t				my_dev;

volatile unsigned int *gpio = NULL; // gpio address mapping table
static unsigned char param[2] = { 0, };
/*
static char msg[BLOCK_SIZE] = {0}; // kernel data buffer to communicate with user app
*/

static int		my_open(struct inode *inode, struct file *filp);
static int		my_release(struct inode *inode, struct file *filp);
/*
static ssize_t	my_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
static ssize_t	my_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
*/
static long my_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

static struct file_operations my_fops = {
	.owner			= THIS_MODULE,
	.open			= my_open,
	.release		= my_release,
/*
	.read			= my_read,
	.write			= my_write,
*/
    .unlocked_ioctl = my_ioctl,
};

static int __init my_init(void)
{
	int ret = 0;
	int minor = 0;
	pr_info("%s():%d\n", __func__, __LINE__);
	if ((ret = alloc_chrdev_region(&my_dev, MINOR_BASE, MINOR_NUM, "mydev")) < 0)
	{
		pr_info("%s():%d alloc_chrdev_region failed (%d)\n", __func__, __LINE__, ret);
		return -EINVAL;
	}
	my_major = MAJOR(my_dev);
	my_dev = MKDEV(my_major, MINOR_BASE);
	cdev_init(&my_cdev, &my_fops);
	my_cdev.owner = THIS_MODULE;
	if ((ret = cdev_add(&my_cdev, my_dev, MINOR_NUM)) < 0)
	{
		pr_info("%s():%d alloc_chrdev_region failed (%d)\n", __func__, __LINE__, ret);
		unregister_chrdev_region(my_dev, MINOR_NUM);
		return -EINVAL;
	}
	my_class = class_create(THIS_MODULE, "mydev");
	if ((ret = IS_ERR(my_class)))
	{
		pr_info("%s():%d class_create failed (%d)\n", __func__, __LINE__, ret);
		cdev_del(&my_cdev);
		unregister_chrdev_region(my_dev, MINOR_NUM);
		return -EINVAL;
	}
	for (minor = MINOR_BASE; minor < MINOR_BASE + MINOR_NUM; minor++)
	{
		if (!device_create(my_class, NULL, MKDEV(my_major, minor), NULL, "mydev%d", minor))
		{
			pr_info("%s():%d device_create failed\n", __func__, __LINE__);
			return -EINVAL;
		}
	}

	return ret;
}

void __exit my_exit(void)
{
	int minor = 0;
	pr_info("%s():%d\n", __func__, __LINE__);
	for (minor = MINOR_BASE; minor < MINOR_BASE + MINOR_NUM; minor++)
	{
		device_destroy(my_class, MKDEV(my_major, minor));
	}
	class_destroy(my_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(my_dev, MINOR_NUM);
}

static int my_open(struct inode *inode, struct file *filp)
{
	pr_info("%s():%d\n", __func__, __LINE__);

    gpio = ioremap(GPIO_BASE, GPIO_SIZE);

	return 0;
}

static int my_release(struct inode *inode, struct file *filp)
{
	pr_info("%s():%d\n", __func__, __LINE__);

	if (gpio)
        iounmap(gpio);

	return 0;
}

/*
static ssize_t my_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
	int ret = 0;
	ret = copy_to_user(buff, msg, strlen(msg) + 1);
	return ret;
}

static ssize_t my_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
	int ret = 0;
	memset(msg, 0, BLOCK_SIZE);
	ret = copy_from_user(msg, buff, count);
	return ret;
}
*/

static long my_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0, size = 0;
    unsigned char gpio_num = 0, gpio_dir = 0, gpio_val = 0;

	pr_info("%s():%d\n", __func__, __LINE__);
	pr_info("%s():%d cmd:%u\n", __func__, __LINE__, cmd);
	
    // ioc verification & exception handling
    if (_IOC_TYPE(cmd) != MYDEV_IOCTL_MAGIC)
	{
		pr_info("%s():%d invalid magic number\n", __func__, __LINE__);
		return -EINVAL;
	}
	if (_IOC_NR(cmd) >= MYDEV_IOCTL_MAX_NR)
	{
		pr_info("%s():%d exceeded max value of cmd\n", __func__, __LINE__);
		return -EINVAL;
	}
	if ((size = _IOC_SIZE(cmd)))
	{
		if (_IOC_DIR(cmd) & _IOC_READ)
		{
			ret = !access_ok(VERIFY_WRITE, (void __user*)arg, size);
		}
		else if (_IOC_DIR(cmd) & _IOC_WRITE)
		{
			ret = !access_ok(VERIFY_READ, (void __user*)arg, size);
		}
		if (ret)
		{
			return ret;
		}
	}
	
    // execute ioc
    switch (cmd)
	{
		case MYDEV_IOCTL_GPIO_DIR:
			pr_info("%s():%d MYDEV_IOCTL_GPIO_DIR\n", __func__, __LINE__);
            ret = copy_from_user((void*)param, (void*)arg, sizeof(param));
            gpio_num = param[0];
            gpio_dir = param[1];
            if (gpio_dir)
                GPIO_OUT(gpio_num);
            else
                GPIO_IN(gpio_num);
			break;
		case MYDEV_IOCTL_GPIO_SET:
			pr_info("%s():%d MYDEV_IOCTL_GPIO_SET\n", __func__, __LINE__);
            ret = copy_from_user((void*)param, (void*)arg, sizeof(param));
            gpio_num = param[0];
            gpio_val = param[1];
            if (gpio_val)
                GPIO_SET(gpio_num);
            else
                GPIO_CLR(gpio_num);
			break;
		case MYDEV_IOCTL_GPIO_GET:
			pr_info("%s():%d MYDEV_IOCTL_GPIO_GET\n",  __func__, __LINE__);
            ret = copy_from_user((void*)&param, (void*)arg, sizeof(param));
            gpio_num = param[0];
            param[0] = GPIO_GET(gpio_num);
	        ret = copy_to_user((void*)arg, (void*)param, sizeof(param));
			break;
		default:
			pr_info("%s():%d unregistered cmd\n", __func__, __LINE__);
			ret = -EINVAL;
			break;
	}
	return ret;
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("lunar456th@naver.com");
MODULE_DESCRIPTION("My Device Driver");
MODULE_VERSION("1.0.0");
