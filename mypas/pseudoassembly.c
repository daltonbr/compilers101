/**@<pseudoassembly.c>::**/

#include <stdio.h>
#include <pseudoassembly.h>

FILE *object;
/*
void headerprog(char *nameProgram)
{
	fprintf(object,"\t .section __TEXT,__text,regular,pure_instructions");
	fprintf(object,"\n\t .global _%s",nameProgram);
}

void rotuloprog(char *nameProgram)
{
	fprintf(object,"\n_%s:\n", nameProgram);
}
*/
int gofalse (int label)
{
    fprintf(object, "\tjz .L%d \t [[gofalse]]\n ", label);
    return label;
}

int jump(int label)
{
    fprintf(object, "\tjmp .L%d \t [[jump]] \n", label);
    return label;
}

int mklabel (int label)
{
    fprintf(object, ".L%d: \t [[mklabel]] \n", label);
    return label;
}

int lmovel(char const *variable)
{
	fprintf(object, "\tmovl %%eax,%s \t [[lmovel]] \n", variable);
	return 0;
}

int lmoveq(char const *variable)
{
	fprintf(object, "\tmovq %%rax,%s \t [[lmoveq]] \n", variable);
	return 0;
}

int rmovel(char const *variable)
{
	fprintf(object, "\tpush %%eax  \t [[rmovel]] \n");
	fprintf(object, "\tmovl %s,%%eax\n", variable);
	return 0;
}

int rmoveq(char const *variable)
{
	fprintf(object, "\tmovq %s, %%rax \t [[rmoveq]] \n", variable);
	return 0;
}


int writeln(char *strwrite)
{
    fprintf(object,"\t movl &4,%%eax\n");
    fprintf(object,"\t movl &1,%%ebx\n");
    fprintf(object,"\t movl %s,%%ecx\n",strwrite);
}

/*ULA pseudo-instructions*/
/*unary*/
int neglog(void)
{
	fprintf(object, "\t \t [[neglog]] \n");

	return 0;
}

int negint(void)
{
fprintf(object, "\t \t [[negint]] \n");

	return 0;
}

int negflt(void)
{
fprintf(object, "\t \t [[negflt]] \n");

	return 0;
}

int negdbl(void)
{
fprintf(object, "\t \t [[negdbl]] \n");

	return 0;
}

int addlog(void)
{
fprintf(object, "\t \t [[addlog]] \n");

	return 0;
}

int addint(void)
{
/*
 * 	addl %eax, (%esp)
 * 	popl %eax
 */
	fprintf(object, "\taddl %%eax, (%%esp) \t [[addint]] \n");
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
	fprintf(object, "\tmovss (%%esp), %%xmm1 \t [[addflt]] \n");
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
	fprintf(object, "\tmovsd (%%rsp), %%xmm1 \t [[adddbl]] \n");
	fprintf(object, "\tmovsd %%rax, %%xmm0\n");
	fprintf(object, "\taddsd %%xmm1, %%xmm0\n");
	fprintf(object, "\tmovsd %%xmm0, %%rax\n");
	fprintf(object, "\taddq $8, %%rsp\n");

	return 0;
}

int sublog(void)
{
fprintf(object, "\t \t [[sublog]] \n");

	return 0;
}

int subint(void)
{
/*
 * 	subl %eax, (%esp)
 * 	popl %eax
 */
	fprintf(object, "\tsubl %%eax, (%%esp) \t [[subint]] \n");
	fprintf(object, "\tpopl %%eax\n");

	return 0;
}

int subflt(void)
{
/*
 * 	movss 	(%esp), %xmm1
 * 	movss 	%eax, %xmm0
 * 	subss 	%xmm1, %xmm0
 * 	movss 	%xmm0, %eax
 * 	addl 	$4, %esp
 */
	fprintf(object, "\tmovss (%%esp), %%xmm1 \t [[subflot]] \n");
	fprintf(object, "\tmovss %%eax, %%xmm0\n");
	fprintf(object, "\tsubss %%xmm1, %%xmm0\n");
	fprintf(object, "\tmovss %%xmm0, %%eax\n");
	fprintf(object, "\taddl $4, %%esp\n");

	return 0;
}

int subdbl(void)
{
/*
 * 	movsd	(%rsp), %xmm1
 * 	movsd	%rax, %xmm0
 * 	subsd	%xmm1, %xmm0
 * 	movsd	%xmm0, %rax
 * 	addq 	$8, %rsp
 */
	fprintf(object, "\tmovsd (%%rsp), %%xmm1 \t [[subdbl]] \n");
	fprintf(object, "\tmovsd %%rax, %%xmm0\n");
	fprintf(object, "\tsubsd %%xmm1, %%xmm0\n");
	fprintf(object, "\tmovsd %%xmm0, %%rax\n");
	fprintf(object, "\taddq $8, %%rsp\n");

	return 0;
}
// and
int mullog(void)
{
fprintf(object, "\t \t [[mullog]] \n");

	return 0;
}


int mulint(void)
{
/*
 * 	subl %eax, (%esp)
 * 	popl %eax
 */
	fprintf(object, "\tmull %%eax, (%%esp) \t [[mulint]] \n");
	fprintf(object, "\tpopl %%eax\n");

	return 0;
}

int mulflt(void)
{
/*
 * 	movss 	(%esp), %xmm1
 * 	movss 	%eax, %xmm0
 * 	mulss 	%xmm1, %xmm0
 * 	movss 	%xmm0, %eax
 * 	addl 	$4, %esp
 */
	fprintf(object, "\tmovss (%%esp), %%xmm1 \t [[mulflt]] \n");
	fprintf(object, "\tmovss %%eax, %%xmm0\n");
	fprintf(object, "\tmulss %%xmm1, %%xmm0\n");
	fprintf(object, "\tmovss %%xmm0, %%eax\n");
	fprintf(object, "\taddl $4, %%esp\n");

	return 0;
}

int muldbl(void)
{
/*
 * 	movsd	(%rsp), %xmm1
 * 	movsd	%rax, %xmm0
 * 	mulsd	%xmm1, %xmm0
 * 	movsd	%xmm0, %rax
 * 	addq 	$8, %rsp
 */
	fprintf(object, "\tmovsd (%%rsp), %%xmm1 \t [[muldbl]] \n");
	fprintf(object, "\tmovsd %%rax, %%xmm0\n");
	fprintf(object, "\tmulsd %%xmm1, %%xmm0\n");
	fprintf(object, "\tmovsd %%xmm0, %%rax\n");
	fprintf(object, "\taddq $8, %%rsp\n");

	return 0;
}

int divint(void)
{
    fprintf(object, "\tdivl (%%esp) \t [[divint]] \n");
	fprintf(object, "\taddl $4, %%esp\n");

    return 0;
}

int divflt(void)
{
    fprintf(object, "\tmovss (%%rsp), %%xmm0 \t [[divflt]] \n");
	fprintf(object, "\tmovss %%rax, %%xmm1\n");
	fprintf(object, "\tdivss %%xmm1, %%xmm0\n");
	fprintf(object, "\tmovss %%xmm0, %%rax\n");
	fprintf(object, "\taddq $8, %%rsp\n");

    return 0;
}

int divdbl(void)
{
    fprintf(object, "\tmovsd (%%rsp), %%xmm0 \t [[divdbl]] \n");
	fprintf(object, "\tmovsd %%rax, %%xmm1\n");
	fprintf(object, "\tdivsd %%xmm1, %%xmm0\n");
	fprintf(object, "\tmovsd %%xmm0, %%rax\n");
	fprintf(object, "\taddq $8, %%rsp\n");

    return 0;
}
