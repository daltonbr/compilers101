  # UNIX assembly factorial function
  # unsigned int factorial (unsigned int n);
  # unsigned int factorial (unsigned int n)
  # {
  #    if ( n < 2 ) return 1;
  #    return factorial(n-1) * n;
  # }
  .section .text
  .global factorial
  .type factorial,@function
  
factorial:
      push %ebp
      movl %esp, %ebp
      cmpl $1, 8(%ebp) # if (n > 1) goto .L1
      ja .L1
      movl $1, %eax # return 1
      jmp .L2
  .L1:
      movl 8(%ebp), %eax # %eax = n
      decl %eax          # %eax--
      push %eax          # parameter is (n-1)
      call factorial     # %eax = factorial (n-1)
      mull 8(%ebp)       # %eax = factorial(n-1) * n
  .L2:
      movl %ebp, %esp # reseting the stack
      pop %ebp
      ret