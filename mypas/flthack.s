	.file	"flthack.c"
	.globl	i
	.data
	.align 4
	.type	i, @object
	.size	i, 4
i:
	.long	3
	.globl	x
	.align 4
	.type	x, @object
	.size	x, 4
x:
	.long	1078530000
	.globl	xx
	.align 8
	.type	xx, @object
	.size	xx, 8
xx:
	.long	4028335726
	.long	1074340345
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	i(%rip), %eax
	addl	$3, %eax
	movl	%eax, i(%rip)
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.3 20140911 (Red Hat 4.8.3-7)"
	.section	.note.GNU-stack,"",@progbits
