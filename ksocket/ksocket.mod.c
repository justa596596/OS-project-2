#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x25475538, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x1e5df697, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x181663c8, __VMLINUX_SYMBOL_STR(sock_setsockopt) },
	{ 0x4c4fef19, __VMLINUX_SYMBOL_STR(kernel_stack) },
	{ 0x9f1038c3, __VMLINUX_SYMBOL_STR(sock_release) },
	{ 0x601c2325, __VMLINUX_SYMBOL_STR(sock_recvmsg) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0xaacde3f7, __VMLINUX_SYMBOL_STR(sock_sendmsg) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0x7ac7fae9, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x4c0bc92e, __VMLINUX_SYMBOL_STR(sock_create) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "763247421284E9A56C49921");
