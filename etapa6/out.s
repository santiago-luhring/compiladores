	.section	.rodata
.LC0:
	.string	"%d"
	.section	.rodata
.LC1:
	.string	"%f"
	.globl	_a
	.data
	.type	_a, @object
	.size	_a, 4
_a:
	.long	0
	.globl	_b
	.data
	.type	_b, @object
	.size	_b, 4
_b:
	.long	2
	.globl	_c
	.data
	.type	_c, @object
	.size	_c, 4
_c:
	.long	6
	.globl	_i
	.data
	.type	_i, @object
	.size	_i, 4
_i:
	.long	10

   #--------------------------TAC_BEGINFUN
	.text
	.globl main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp

   #--------------------------TAC_COPY
	movl _b(%rip), %eax
	movl %eax, _a(%rip)

   #--------------------------TAC_COPY
	movl _c(%rip), %eax
	movl %eax, _b(%rip)

   #--------------------------TAC_COPY
	movl _i(%rip), %eax
	movl %eax, _c(%rip)

   #--------------------------TAC_PRINT
	movl	_c(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

   #--------------------------TAC_COPY
	movl _a(%rip), %eax
	movl %eax, _i(%rip)

   #--------------------------TAC_ENDFUN
	popq	%rbp
	ret
