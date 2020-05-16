#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x9bf801d1, "module_layout" },
	{ 0x593c35b1, "class_destroy" },
	{ 0x8a8f2c5, "device_destroy" },
	{ 0x70dafcf7, "device_create" },
	{ 0x61bb93f, "cdev_del" },
	{ 0x33b0afe6, "__class_create" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x57c3155f, "cdev_add" },
	{ 0x3c4d59d3, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xf4fa543b, "arm_copy_to_user" },
	{ 0x5f754e5a, "memset" },
	{ 0x28cc25db, "arm_copy_from_user" },
	{ 0xe97c4103, "ioremap" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xedc03953, "iounmap" },
	{ 0x7c32d0f0, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "8B7C9ACD3C3B27B7CC55FBA");
