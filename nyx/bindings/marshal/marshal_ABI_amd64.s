	.file	"marshal_ABI_amd64.c"
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
	pushq	%rbx
	subq	$264, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -232(%rbp)
	movq	%rsi, -240(%rbp)
	movq	%rdx, -248(%rbp)
	movq	%rcx, -256(%rbp)
	movq	%r8, -264(%rbp)
	movq	-264(%rbp), %rax
	movq	%rax, %rdi
	call	node_GetItemsNum@PLT
	movq	%rax, -88(%rbp)
	movq	-240(%rbp), %rax
	leaq	.LC0(%rip), %rsi
	movq	%rax, %rdi
	call	node_GetItemByKey@PLT
	movq	%rax, -96(%rbp)
	movq	-96(%rbp), %rax
	leaq	.LC1(%rip), %rsi
	movq	%rax, %rdi
	call	node_GetItemByKey@PLT
	movq	%rax, -104(%rbp)
	movq	-240(%rbp), %rax
	leaq	.LC2(%rip), %rsi
	movq	%rax, %rdi
	call	get_member@PLT
	movq	%rax, -112(%rbp)
	movq	$0, -24(%rbp)
	cmpq	$0, -112(%rbp)
	je	.L2
	movq	-112(%rbp), %rax
	movq	%rax, %rdi
	call	resolve_object@PLT
	movq	%rax, %rdi
	call	get_value@PLT
	movq	%rax, %rdi
	call	node_GetSint32@PLT
	movq	%rax, -24(%rbp)
.L2:
	movq	-104(%rbp), %rax
	movq	%rax, %rdi
	call	node_GetValue@PLT
	movq	%rax, -120(%rbp)
	movq	$0, -128(%rbp)
	movq	$0, -136(%rbp)
	movq	$0, -32(%rbp)
	movq	$0, -40(%rbp)
	movq	$0, -48(%rbp)
	movq	$0, -56(%rbp)
	movq	$0, -64(%rbp)
	movq	$0, -72(%rbp)
	movq	$0, -80(%rbp)
	movq	$0, -144(%rbp)
	movq	$0, -152(%rbp)
	movq	-88(%rbp), %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, -160(%rbp)
	movq	-88(%rbp), %rax
	salq	$3, %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, -168(%rbp)
	movq	-264(%rbp), %rax
	movq	%rax, %rdi
	call	node_ItemReverseIterationReset@PLT
	jmp	.L3
.L6:
	movq	-264(%rbp), %rax
	movq	%rax, %rdi
	call	node_ItemReverseIterate@PLT
	movq	%rax, -176(%rbp)
	movq	-176(%rbp), %rax
	movq	%rax, %rdi
	call	get_value@PLT
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %rax
	movq	%rax, %rdi
	call	node_GetUint64@PLT
	movq	%rax, -192(%rbp)
	movq	-184(%rbp), %rax
	movq	%rax, %rdi
	call	node_GetType@PLT
	cmpb	$3, %al
	jne	.L4
	movq	-56(%rbp), %rdx
	movq	-160(%rbp), %rax
	addq	%rdx, %rax
	movb	$1, (%rax)
	addq	$1, -64(%rbp)
	jmp	.L5
.L4:
	movq	-56(%rbp), %rdx
	movq	-160(%rbp), %rax
	addq	%rdx, %rax
	movb	$0, (%rax)
.L5:
	movq	-56(%rbp), %rax
	leaq	0(,%rax,8), %rdx
	movq	-168(%rbp), %rax
	addq	%rax, %rdx
	movq	-192(%rbp), %rax
	movq	%rax, (%rdx)
	addq	$1, -56(%rbp)
.L3:
	movq	-264(%rbp), %rax
	movq	%rax, %rdi
	call	node_ItemReverseIterationUnfinished@PLT
	testl	%eax, %eax
	jne	.L6
	movq	-64(%rbp), %rax
	movq	-88(%rbp), %rdx
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, -144(%rbp)
	movq	-64(%rbp), %rax
	subq	$8, %rax
	movq	%rax, -72(%rbp)
	movq	-144(%rbp), %rax
	subq	$6, %rax
	movq	%rax, -80(%rbp)
	cmpq	$0, -72(%rbp)
	jns	.L7
	movq	$0, -72(%rbp)
.L7:
	cmpq	$0, -80(%rbp)
	jns	.L8
	movq	$0, -80(%rbp)
.L8:
	movq	-80(%rbp), %rdx
	movq	-72(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -152(%rbp)
#APP
# 78 "marshal_ABI_amd64.c" 1
	movq %rsp,%rax
	
# 0 "" 2
#NO_APP
	movq	%rax, -136(%rbp)
	movq	-64(%rbp), %rax
	subq	$1, %rax
	movq	%rax, -32(%rbp)
	movq	-144(%rbp), %rax
	subq	$1, %rax
	movq	%rax, -40(%rbp)
	movq	-152(%rbp), %rax
	cqto
	shrq	$63, %rdx
	addq	%rdx, %rax
	andl	$1, %eax
	subq	%rdx, %rax
	cmpq	$1, %rax
	jne	.L9
#APP
# 88 "marshal_ABI_amd64.c" 1
	subq $8,%rsp
	
# 0 "" 2
#NO_APP
.L9:
	jmp	.L10
.L29:
	movq	-48(%rbp), %rax
	leaq	0(,%rax,8), %rbx
	movq	-168(%rbp), %rax
	addq	%rbx, %rax
	movq	(%rax), %rax
	movq	%rax, -200(%rbp)
	movq	-48(%rbp), %rbx
	movq	-160(%rbp), %rax
	addq	%rbx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L11
	cmpq	$0, -32(%rbp)
	jne	.L12
	movq	-200(%rbp), %rax
#APP
# 99 "marshal_ABI_amd64.c" 1
	movq %rax, %xmm0
	
# 0 "" 2
#NO_APP
	jmp	.L13
.L12:
	cmpq	$1, -32(%rbp)
	jne	.L14
	movq	-200(%rbp), %rax
#APP
# 101 "marshal_ABI_amd64.c" 1
	movq %rax, %xmm1
	
# 0 "" 2
#NO_APP
	jmp	.L13
.L14:
	cmpq	$2, -32(%rbp)
	jne	.L15
	movq	-200(%rbp), %rax
#APP
# 103 "marshal_ABI_amd64.c" 1
	movq %rax, %xmm2
	
# 0 "" 2
#NO_APP
	jmp	.L13
.L15:
	cmpq	$3, -32(%rbp)
	jne	.L16
	movq	-200(%rbp), %rax
#APP
# 105 "marshal_ABI_amd64.c" 1
	movq %rax, %xmm3
	
# 0 "" 2
#NO_APP
	jmp	.L13
.L16:
	cmpq	$4, -32(%rbp)
	jne	.L17
	movq	-200(%rbp), %rax
#APP
# 107 "marshal_ABI_amd64.c" 1
	movq %rax, %xmm4
	
# 0 "" 2
#NO_APP
	jmp	.L13
.L17:
	cmpq	$5, -32(%rbp)
	jne	.L18
	movq	-200(%rbp), %rax
#APP
# 109 "marshal_ABI_amd64.c" 1
	movq %rax, %xmm5
	
# 0 "" 2
#NO_APP
	jmp	.L13
.L18:
	cmpq	$6, -32(%rbp)
	jne	.L19
	movq	-200(%rbp), %rax
#APP
# 111 "marshal_ABI_amd64.c" 1
	movq %rax, %xmm6
	
# 0 "" 2
#NO_APP
	jmp	.L13
.L19:
	cmpq	$7, -32(%rbp)
	jne	.L20
	movq	-200(%rbp), %rax
#APP
# 113 "marshal_ABI_amd64.c" 1
	movq %rax, %xmm7
	
# 0 "" 2
#NO_APP
	jmp	.L13
.L20:
	movq	-200(%rbp), %rax
#APP
# 115 "marshal_ABI_amd64.c" 1
	pushq %rax
	
# 0 "" 2
#NO_APP
.L13:
	subq	$1, -32(%rbp)
	jmp	.L21
.L11:
	cmpq	$0, -40(%rbp)
	jne	.L22
	movq	-200(%rbp), %rax
#APP
# 121 "marshal_ABI_amd64.c" 1
	movq %rax,%rdi
	
# 0 "" 2
#NO_APP
	jmp	.L23
.L22:
	cmpq	$1, -40(%rbp)
	jne	.L24
	movq	-200(%rbp), %rax
#APP
# 123 "marshal_ABI_amd64.c" 1
	movq %rax,%rsi
	
# 0 "" 2
#NO_APP
	jmp	.L23
.L24:
	cmpq	$2, -40(%rbp)
	jne	.L25
	movq	-200(%rbp), %rax
#APP
# 125 "marshal_ABI_amd64.c" 1
	movq %rax,%rdx
	
# 0 "" 2
#NO_APP
	jmp	.L23
.L25:
	cmpq	$3, -40(%rbp)
	jne	.L26
	movq	-200(%rbp), %rax
#APP
# 127 "marshal_ABI_amd64.c" 1
	movq %rax,%rcx
	
# 0 "" 2
#NO_APP
	jmp	.L23
.L26:
	cmpq	$4, -40(%rbp)
	jne	.L27
	movq	-200(%rbp), %rax
#APP
# 129 "marshal_ABI_amd64.c" 1
	movq %rax,%r8
	
# 0 "" 2
#NO_APP
	jmp	.L23
.L27:
	cmpq	$5, -40(%rbp)
	jne	.L28
	movq	-200(%rbp), %rax
#APP
# 131 "marshal_ABI_amd64.c" 1
	movq %rax,%r9
	
# 0 "" 2
#NO_APP
	jmp	.L23
.L28:
	movq	-200(%rbp), %rax
#APP
# 135 "marshal_ABI_amd64.c" 1
	pushq %rax
	
# 0 "" 2
#NO_APP
.L23:
	subq	$1, -40(%rbp)
.L21:
	addq	$1, -48(%rbp)
.L10:
	movq	-48(%rbp), %rax
	cmpq	-88(%rbp), %rax
	jb	.L29
	cmpq	$8, -64(%rbp)
	jle	.L30
	movq	$8, -64(%rbp)
.L30:
	movq	-64(%rbp), %rax
	movq	-120(%rbp), %rbx
#APP
# 145 "marshal_ABI_amd64.c" 1
	call *%rbx
	
# 0 "" 2
#NO_APP
	movq	%rax, -128(%rbp)
	movq	-136(%rbp), %rax
#APP
# 154 "marshal_ABI_amd64.c" 1
	movq %rax,%rsp
	
# 0 "" 2
#NO_APP
	movq	-232(%rbp), %rax
	movq	%rax, %rdi
	call	get_base_class@PLT
	movq	%rax, -208(%rbp)
	movq	-208(%rbp), %rax
	movq	%rax, %rdi
	call	create_class_instance@PLT
	movq	%rax, -216(%rbp)
	movq	-216(%rbp), %rax
	leaq	.LC3(%rip), %rdx
	leaq	.LC4(%rip), %rsi
	movq	%rax, %rdi
	call	set_obj_string@PLT
	movq	-216(%rbp), %rdx
	movq	-232(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	add_garbage@PLT
	movq	-216(%rbp), %rax
	movq	%rax, %rdi
	call	get_value@PLT
	movq	%rax, -224(%rbp)
	movq	-128(%rbp), %rdx
	movq	-224(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	node_SetUint64@PLT
	movq	-224(%rbp), %rax
	movl	$10, %esi
	movq	%rax, %rdi
	call	node_SetType@PLT
	cmpq	$1, -24(%rbp)
	jne	.L31
	movq	-224(%rbp), %rax
	movl	$3, %esi
	movq	%rax, %rdi
	call	node_SetType@PLT
.L31:
	cmpq	$2, -24(%rbp)
	jne	.L32
	movq	-224(%rbp), %rax
	movl	$13, %esi
	movq	%rax, %rdi
	call	node_SetType@PLT
.L32:
	movq	-160(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movq	-168(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movq	-216(%rbp), %rax
	addq	$264, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	marshal_call_function, .-marshal_call_function
	.ident	"GCC: (Debian 4.9.1-15) 4.9.1"
	.section	.note.GNU-stack,"",@progbits
