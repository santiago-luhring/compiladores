a
	.globl	__temp0
	.data
	.type	__temp0, @object
	.size	__temp0, 4
__temp0:
	.long  0
	.section	.rodata
.LC0:
	.string	"%d"
	.section	.rodata
.LC1:
	.string	"%f"
	.text
	.globl main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl _0(%rip), %eax
	movl %eax, _a(%rip)
	movl _1(%rip), %eax
	movl %eax, _b(%rip)
	movl _5(%rip), %eax
	movl %eax, _c(%rip)
	movl _c(%rip), %eax
	movl _55(%rip), %edx
	cltq
	movl %edx, _v(,%rax, 4)
	movl _5(%rip), %eax
	cltq
	movl _v(,%rax, 4), %eax
	movl %eax, __temp0(%rip)
	movl	__temp0(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	_c(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl _2(%rip), %eax
	movl %eax, _i(%rip)
	popq	%rbp
	ret
