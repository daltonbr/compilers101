	.file	"floatingsamples.c"
	.globl	x
	.data
	.align 4
	.type	x, @object
	.size	x, 4
x:
	.long	1078530000
	.comm	cx,4,1
	.globl	y
	.align 8
	.type	y, @object
	.size	y, 8
y:
	.long	4028335726
	.long	1074340345
	.comm	cy,8,1
	.section	.rodata
.LC0:
	.string	"%.6f %.15lf\n"
.LC1:
	.string	"%02x"
.LC2:
	.string	"\n%u\n"
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
	subq	$32, %rsp
	movl	x(%rip), %eax
	movl	%eax, cx(%rip)
	movq	y(%rip), %rax
	movq	%rax, cy(%rip)
	movq	y(%rip), %rax
	movss	x(%rip), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm0
	movq	%rax, -24(%rbp)
	movsd	-24(%rbp), %xmm1
	movl	$.LC0, %edi
	movl	$2, %eax
	call	printf
	movl	$0, -4(%rbp)
	jmp	.L2
.L3:
	movl	$3, %eax
	subl	-4(%rbp), %eax
	cltq
	movzbl	cx(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %esi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	addl	$1, -4(%rbp)
.L2:
	cmpl	$3, -4(%rbp)
	jle	.L3
	movl	$x, %eax
	movl	(%rax), %eax
	movl	%eax, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	movl	$10, %edi
	call	putchar
	movl	$0, -4(%rbp)
	jmp	.L4
.L5:
	movl	$7, %eax
	subl	-4(%rbp), %eax
	cltq
	movzbl	cy(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %esi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	addl	$1, -4(%rbp)
.L4:
	cmpl	$7, -4(%rbp)
	jle	.L5
	movl	$y, %eax
	movl	(%rax), %eax
	movl	%eax, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	movl	$y+4, %eax
	movl	(%rax), %eax
	movl	%eax, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	movl	$10, %edi
	call	putchar
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.3 20140911 (Red Hat 4.8.3-7)"
	.section	.note.GNU-stack,"",@progbits
