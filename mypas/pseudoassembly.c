
int lmovel(char const *variable)
{
	fprintf(object, "\tmovl %%eax,%s\n", variable;
	return 0;
}

int lmoveq(char const *variable)
{
	fprintf(object, "\tmovq %%rax,%s\n", variable;
	return 0;
}

int rmovel(char const *variable)
{
	fprintf(object, "\tmovl %s,%%eax\n", variable;
	return 0;
}

int rmoveq(char const *variable)
{
	fprintf(object, "\tmovq %s, %%rax\n", variable;
	return 0;
}

/*ULA pseudo-instructions*/
/*unary*/
int neglog(void)
{

}

int negint(void)
{

}

int negflt(void)
{

}

int negdbl(void)
{

}

int addlog(void)
{

}

int addint(void)
{
/*
 * 	addl %eax, (%esp)
 * 	popl %eax
 */
	fprintf(object, "\taddl %%eax, (%%esp)\n");
	fprintf(object, "\tpopl %%eax\n");

	return 0;
}

int addflt(void)
{
/*
 * 	movss 	(%esp), %xmm1
 * 	movss 	%eax, %xmm0
 * 	addss 	%xmm1, %xmm0
 * 	movss 	%xmm0, %eax
 * 	addl 	$4, %esp
 */
	fprintf(object, "\tmovss (%%esp), %%xmm1\n");
	fprintf(object, "\tmovss %%eax, %%xmm0\n");
	fprintf(object, "\taddss %%xmm1, %%xmm0\n");
	fprintf(object, "\tmovss %%xmm0, %%eax\n");
	fprintf(object, "\taddl $4, %%esp\n");

	return 0;
}

int adddbl(void)
{
/*
 * 	movsd	(%rsp), %xmm1
 * 	movsd	%rax, %xmm0
 * 	addsd	%xmm1, %xmm0
 * 	movsd	%xmm0, %rax
 * 	addq 	$8, %rsp
 */
	fprintf(object, "\tmovsd   xx(%rsp), %xmm1\n");
	fprintf(object, "\tmovsd   %rax, %xmm0\n");
	fprintf(object, "\taddsd   %xmm1, %xmm0\n");
	fprintf(object, "\tmovsd   %xmm0, %rax\n");
	fprintf(object, "\taddq    $8, %%rsp\n");

	return 0;
}

int sublog(void)
{

}

int subint(void)
{

}

int subflt(void)
{

}

int subdbl(void)
{

}

int mullog(void)
{

}

int mulint(void)
{

}

int mulflt(void)
{

}

int muldbl(void)
{

}

int divint(void)
{

}

int divflt(void)
{

}

int divdbl(void)
{

}
