#define MAGIC        0x1badb002
#define FLAGS        0x7

.section .multiboot
	.align           4
	.long            MAGIC
	.long            FLAGS
	.long            -(MAGIC + FLAGS)

.section .text   /* multiboot info */
.global _start   /* magic */
_start:	          /* kernel entry point */
	push %ebx
	push %eax
	call main
	hlt
	
