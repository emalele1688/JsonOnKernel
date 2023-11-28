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

KSYMTAB_FUNC(kjson_new_container, "_gpl", "");
KSYMTAB_FUNC(kjson_delete_container, "_gpl", "");
KSYMTAB_FUNC(kjson_delete_object, "", "");
KSYMTAB_FUNC(kjson_lookup_object, "_gpl", "");
KSYMTAB_FUNC(kjson_pop_object, "", "");
KSYMTAB_FUNC(__kjson_push_object, "_gpl", "");
KSYMTAB_FUNC(kjson_push_object, "_gpl", "");
KSYMTAB_FUNC(kjson_dump, "_gpl", "");
KSYMTAB_FUNC(kjson_parse, "_gpl", "");

SYMBOL_CRC(kjson_new_container, 0x3011da0f, "_gpl");
SYMBOL_CRC(kjson_delete_container, 0x92a2a3c1, "_gpl");
SYMBOL_CRC(kjson_delete_object, 0x5f90651e, "");
SYMBOL_CRC(kjson_lookup_object, 0xfe6e498d, "_gpl");
SYMBOL_CRC(kjson_pop_object, 0x2d1bb97c, "");
SYMBOL_CRC(__kjson_push_object, 0xffa311ff, "_gpl");
SYMBOL_CRC(kjson_push_object, 0x72a22737, "_gpl");
SYMBOL_CRC(kjson_dump, 0x0802027f, "_gpl");
SYMBOL_CRC(kjson_parse, 0x39d46e7b, "_gpl");

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x656e4a6e, "snprintf" },
	{ 0x69acdf38, "memcpy" },
	{ 0x37a0cba, "kfree" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0x122c3a7e, "_printk" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xa916b694, "strnlen" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0x9166fada, "strncpy" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x56470118, "__warn_printk" },
	{ 0x5e6c2443, "kmalloc_trace" },
	{ 0x3854774b, "kstrtoll" },
	{ 0x54b1fac6, "__ubsan_handle_load_invalid_value" },
	{ 0x754d539c, "strlen" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0xf924854e, "kmalloc_caches" },
	{ 0x101fd29a, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "563FA89C568D7218ACA5753");
