#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
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



static const char ____versions[]
__used __section("__versions") =
	"\x14\x00\x00\x00\xa8\x60\xf3\x74"
	"kern_path\0\0\0"
	"\x14\x00\x00\x00\xd2\x19\xbc\x57"
	"down_write\0\0"
	"\x14\x00\x00\x00\x25\x7a\x80\xce"
	"up_write\0\0\0\0"
	"\x1c\x00\x00\x00\xcb\xf6\xfd\xf0"
	"__stack_chk_fail\0\0\0\0"
	"\x14\x00\x00\x00\x92\x31\x83\x18"
	"filp_open\0\0\0"
	"\x18\x00\x00\x00\x3d\x73\xae\xe5"
	"kernel_write\0\0\0\0"
	"\x14\x00\x00\x00\x31\xb7\x42\x2d"
	"filp_close\0\0"
	"\x1c\x00\x00\x00\xc4\xdc\xee\xa7"
	"call_usermodehelper\0"
	"\x1c\x00\x00\x00\xc6\xaf\xbb\x89"
	"usb_register_notify\0"
	"\x20\x00\x00\x00\x34\xc3\x1d\x81"
	"usb_unregister_notify\0\0\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x18\x00\x00\x00\x3a\x0a\xd8\xfc"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "ADCB9CBCA510432A1DA014E");
