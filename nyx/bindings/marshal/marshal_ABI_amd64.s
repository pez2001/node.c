#	.file	"marshal_ABI_amd64.c"
	.section	.rodata
.LC0:
	.string	"privates"
.LC1:
	.string	"marshal_sym"
.LC2:
	.string	"return_type"
.LC3:
	.string	"marshal.return_value"
.LC4:
	.string	"name"
	.text
	.globl	marshal_call_function
	.type	marshal_call_function, @function
marshal_call_function:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$224, %rsp
	movq	%rdi, -184(%rbp)
	movq	%rsi, -192(%rbp)
	movq	%rdx, -200(%rbp)
	movq	%rcx, -208(%rbp)
	movq	%r8, -216(%rbp)
	movq	-216(%rbp), %rax
	movq	%rax, %rdi
	call	node_GetItemsNum
	movq	%rax, -48(%rbp)
	movq	-192(%rbp), %rax
	movl	$.LC0, %esi
	movq	%rax, %rdi
	call	node_GetItemByKey
	movq	%rax, -56(%rbp)
	movq	-56(%rbp), %rax
	movl	$.LC1, %esi
	movq	%rax, %rdi
	call	node_GetItemByKey
	movq	%rax, -64(%rbp)
	movq	-192(%rbp), %rax
	movl	$.LC2, %esi
	movq	%rax, %rdi
	call	get_member
	movq	%rax, -72(%rbp)
	movq	$0, -8(%rbp)
	cmpq	$0, -72(%rbp)
	je	.L2
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	resolve_object
	movq	%rax, %rdi
	call	get_value
	movq	%rax, %rdi
	call	node_GetSint32
	movq	%rax, -8(%rbp)
.L2:
	movq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	node_GetValue
	movq	%rax, -80(%rbp)
	movq	$0, -88(%rbp)
	movq	$0, -96(%rbp)
	movq	$0, -16(%rbp)
	movq	$0, -24(%rbp)
	movq	$0, -32(%rbp)
	movq	$0, -40(%rbp)
	movq	-48(%rbp), %rax
	salq	$3, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -104(%rbp)
	movq	-48(%rbp), %rax
	salq	$3, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -112(%rbp)
	movq	-48(%rbp), %rax
	salq	$3, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -120(%rbp)
	movq	-216(%rbp), %rax
	movq	%rax, %rdi
	call	node_ItemIterationReset
	jmp	.L3
.L8:
	movq	-216(%rbp), %rax
	movq	%rax, %rdi
	call	node_ItemIterate
	movq	%rax, -128(%rbp)
	movq	-128(%rbp), %rax
	movq	%rax, %rdi
	call	get_value
	movq	%rax, -136(%rbp)
	movq	-136(%rbp), %rax
	movq	%rax, %rdi
	call	node_GetUint64
	movq	%rax, -144(%rbp)
	movq	-136(%rbp), %rax
	movq	%rax, %rdi
	call	node_GetType
	cmpb	$3, %al
	jne	.L4
	cmpq	$8, -16(%rbp)
	jg	.L5
	movq	-16(%rbp), %rax
	leaq	0(,%rax,8), %rdx
	movq	-120(%rbp), %rax
	addq	%rax, %rdx
	movq	-144(%rbp), %rax
	movq	%rax, (%rdx)
	addq	$1, -16(%rbp)
	jmp	.L3
.L5:
	movq	-32(%rbp), %rax
	leaq	0(,%rax,8), %rdx
	movq	-104(%rbp), %rax
	addq	%rax, %rdx
	movq	-144(%rbp), %rax
	movq	%rax, (%rdx)
	addq	$1, -32(%rbp)
	jmp	.L3
.L4:
	cmpq	$6, -24(%rbp)
	jg	.L7
	movq	-24(%rbp), %rax
	leaq	0(,%rax,8), %rdx
	movq	-112(%rbp), %rax
	addq	%rax, %rdx
	movq	-144(%rbp), %rax
	movq	%rax, (%rdx)
	addq	$1, -24(%rbp)
	jmp	.L3
.L7:
	movq	-32(%rbp), %rax
	leaq	0(,%rax,8), %rdx
	movq	-104(%rbp), %rax
	addq	%rax, %rdx
	movq	-144(%rbp), %rax
	movq	%rax, (%rdx)
	addq	$1, -32(%rbp)
.L3:
	movq	-216(%rbp), %rax
	movq	%rax, %rdi
	call	node_ItemIterationUnfinished
	testl	%eax, %eax
	jne	.L8
#APP
# 83 "marshal_ABI_amd64.c" 1
	movq %rsp,%rax
	
# 0 "" 2
#NO_APP
	movq	%rax, -96(%rbp)
	cmpq	$0, -32(%rbp)
	je	.L9
	movq	-32(%rbp), %rax
	subq	$1, %rax
	movq	%rax, -40(%rbp)
	jmp	.L10
.L11:
	movq	-40(%rbp), %rax
	leaq	-1(%rax), %rdx
	movq	%rdx, -40(%rbp)
	leaq	0(,%rax,8), %rdx
	movq	-104(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
#APP
# 94 "marshal_ABI_amd64.c" 1
	pushq %rax
	
# 0 "" 2
#NO_APP
.L10:
	cmpq	$0, -40(%rbp)
	jns	.L11
.L9:
	cmpq	$0, -16(%rbp)
	jle	.L12
	movq	-120(%rbp), %rax
#APP
# 98 "marshal_ABI_amd64.c" 1
	movsd (%rax), %xmm0
	
# 0 "" 2
#NO_APP
	jmp	.L13
.L12:
	cmpq	$1, -16(%rbp)
	jle	.L14
	movq	-120(%rbp), %rax
	addq	$8, %rax
#APP
# 100 "marshal_ABI_amd64.c" 1
	movsd (%rax), %xmm1
	
# 0 "" 2
#NO_APP
	jmp	.L13
.L14:
	cmpq	$2, -16(%rbp)
	jle	.L15
	movq	-120(%rbp), %rax
	addq	$16, %rax
#APP
# 102 "marshal_ABI_amd64.c" 1
	movsd (%rax), %xmm2
	
# 0 "" 2
#NO_APP
	jmp	.L13
.L15:
	cmpq	$3, -16(%rbp)
	jle	.L16
	movq	-120(%rbp), %rax
	addq	$24, %rax
#APP
# 104 "marshal_ABI_amd64.c" 1
	movsd (%rax), %xmm3
	
# 0 "" 2
#NO_APP
	jmp	.L13
.L16:
	cmpq	$4, -16(%rbp)
	jle	.L17
	movq	-120(%rbp), %rax
	addq	$32, %rax
#APP
# 106 "marshal_ABI_amd64.c" 1
	movsd (%rax), %xmm4
	
# 0 "" 2
#NO_APP
	jmp	.L13
.L17:
	cmpq	$5, -16(%rbp)
	jle	.L18
	movq	-120(%rbp), %rax
	addq	$40, %rax
#APP
# 108 "marshal_ABI_amd64.c" 1
	movsd (%rax), %xmm5
	
# 0 "" 2
#NO_APP
	jmp	.L13
.L18:
	cmpq	$6, -16(%rbp)
	jle	.L19
	movq	-120(%rbp), %rax
	addq	$48, %rax
#APP
# 110 "marshal_ABI_amd64.c" 1
	movsd (%rax), %xmm6
	
# 0 "" 2
#NO_APP
	jmp	.L13
.L19:
	cmpq	$7, -16(%rbp)
	jle	.L13
	movq	-120(%rbp), %rax
	addq	$56, %rax
#APP
# 112 "marshal_ABI_amd64.c" 1
	movsd (%rax), %xmm7
	
# 0 "" 2
#NO_APP
.L13:
	cmpq	$0, -24(%rbp)
	jle	.L20
	movq	-112(%rbp), %rax
#APP
# 116 "marshal_ABI_amd64.c" 1
	movq (%rax),%rdi
	
# 0 "" 2
#NO_APP
	jmp	.L21
.L20:
	cmpq	$1, -24(%rbp)
	jle	.L22
	movq	-112(%rbp), %rax
	addq	$8, %rax
#APP
# 118 "marshal_ABI_amd64.c" 1
	movq (%rax),%rsi
	
# 0 "" 2
#NO_APP
	jmp	.L21
.L22:
	cmpq	$2, -24(%rbp)
	jle	.L23
	movq	-112(%rbp), %rax
	addq	$16, %rax
#APP
# 120 "marshal_ABI_amd64.c" 1
	movq (%rax),%rdx
	
# 0 "" 2
#NO_APP
	jmp	.L21
.L23:
	cmpq	$3, -24(%rbp)
	jle	.L24
	movq	-112(%rbp), %rax
	addq	$24, %rax
#APP
# 122 "marshal_ABI_amd64.c" 1
	movq (%rax),%rcx
	
# 0 "" 2
#NO_APP
	jmp	.L21
.L24:
	cmpq	$4, -24(%rbp)
	jle	.L25
	movq	-112(%rbp), %rax
	addq	$32, %rax
#APP
# 124 "marshal_ABI_amd64.c" 1
	movq (%rax),%r8
	
# 0 "" 2
#NO_APP
	jmp	.L21
.L25:
	cmpq	$5, -24(%rbp)
	jle	.L21
	movq	-112(%rbp), %rax
	addq	$40, %rax
#APP
# 126 "marshal_ABI_amd64.c" 1
	movq (%rax),%r9
	
# 0 "" 2
#NO_APP
.L21:
#	movq	-48(%rbp), %rdx
	movq	-80(%rbp), %rax
#APP
# 129 "marshal_ABI_amd64.c" 1
#	movq %rdx,%rax
	call %rax
	movq %rax,%rdx
	
# 0 "" 2
#NO_APP
	movq	%rdx, -88(%rbp)
	movq	-96(%rbp), %rax
#APP
# 137 "marshal_ABI_amd64.c" 1
	movq %rax,%rsp
	
# 0 "" 2
#NO_APP
	movq	-184(%rbp), %rax
	movq	%rax, %rdi
	call	get_base_class
	movq	%rax, -152(%rbp)
	movq	-152(%rbp), %rax
	movq	%rax, %rdi
	call	create_class_instance
	movq	%rax, -160(%rbp)
	movq	-160(%rbp), %rax
	movl	$.LC3, %edx
	movl	$.LC4, %esi
	movq	%rax, %rdi
	call	set_obj_string
	movq	-160(%rbp), %rdx
	movq	-184(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	add_garbage
	movq	-160(%rbp), %rax
	movq	%rax, %rdi
	call	get_value
	movq	%rax, -168(%rbp)
	movq	-88(%rbp), %rdx
	movq	-168(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	node_SetUint64
	movq	-168(%rbp), %rax
	movl	$10, %esi
	movq	%rax, %rdi
	call	node_SetType
	cmpq	$1, -8(%rbp)
	jne	.L26
	movq	-168(%rbp), %rax
	movl	$3, %esi
	movq	%rax, %rdi
	call	node_SetType
.L26:
	movq	-104(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movq	-112(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movq	-120(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movq	-160(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	marshal_call_function, .-marshal_call_function
	.ident	"GCC: (Debian 4.9.1-15) 4.9.1"
	.section	.note.GNU-stack,"",@progbits
