	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 12
	.section	__TEXT,__literal4,4byte_literals
	.align	2
LCPI0_0:
	.long	1114636288              ## float 60
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp0:
	.cfi_def_cfa_offset 16
Ltmp1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp2:
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	movss	LCPI0_0(%rip), %xmm0    ## xmm0 = mem[0],zero,zero,zero
	movl	$0, -4(%rbp)
	imull	$60, _i(%rip), %ecx
	movl	%ecx, _i(%rip)
	mulss	_x(%rip), %xmm0
	movss	%xmm0, _x(%rip)
	movsd	_xx(%rip), %xmm0        ## xmm0 = mem[0],zero
	movd	%xmm0, %rdx
	movabsq	$-9223372036854775808, %rsi ## imm = 0x8000000000000000
	xorq	%rsi, %rdx
	movd	%rdx, %xmm0
	movsd	%xmm0, _xx(%rip)
	popq	%rbp
	retq
	.cfi_endproc

	.section	__DATA,__data
	.globl	_i                      ## @i
	.align	2
_i:
	.long	3                       ## 0x3

	.globl	_x                      ## @x
	.align	2
_x:
	.long	1078530000              ## float 3.14159012

	.globl	_xx                     ## @xx
	.align	3
_xx:
	.quad	4614256650576692846     ## double 3.1415899999999999


.subsections_via_symbols
