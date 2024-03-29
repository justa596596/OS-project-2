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
	{ 0xc458d0e1, __VMLINUX_SYMBOL_STR(debugfs_remove) },
	{ 0xd87b189d, __VMLINUX_SYMBOL_STR(misc_deregister) },
	{ 0x304c34ff, __VMLINUX_SYMBOL_STR(misc_register) },
	{ 0x753d28c9, __VMLINUX_SYMBOL_STR(debugfs_create_file) },
	{ 0xa202a8e5, __VMLINUX_SYMBOL_STR(kmalloc_order_trace) },
	{ 0x10cde6fd, __VMLINUX_SYMBOL_STR(inet_ntoa) },
	{ 0x3b1d969b, __VMLINUX_SYMBOL_STR(kconnect) },
	{ 0x83d05d4d, __VMLINUX_SYMBOL_STR(ksocket) },
	{ 0x84856d65, __VMLINUX_SYMBOL_STR(inet_addr) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xc05d0e27, __VMLINUX_SYMBOL_STR(kclose) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x960a999e, __VMLINUX_SYMBOL_STR(pv_mmu_ops) },
	{ 0xeb080a32, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x4c4fef19, __VMLINUX_SYMBOL_STR(kernel_stack) },
	{ 0x5321adb3, __VMLINUX_SYMBOL_STR(remap_pfn_range) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x8b7e8136, __VMLINUX_SYMBOL_STR(krecv) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xe28c5c36, __VMLINUX_SYMBOL_STR(__get_page_tail) },
	{ 0x4c9d28b0, __VMLINUX_SYMBOL_STR(phys_base) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=ksocket";


MODULE_INFO(srcversion, "DA5D0D6B59CC7C99BFB4FD8");
