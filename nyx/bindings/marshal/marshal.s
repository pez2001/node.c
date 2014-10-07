	.file	"marshal.c"
	.section	.rodata
.LC0:
	.string	"marshal"
.LC1:
	.string	"="
.LC2:
	.string	"test"
.LC3:
	.string	"open_module"
.LC4:
	.string	"close_module"
.LC5:
	.string	"open_lib"
.LC6:
	.string	"close_lib"
.LC7:
	.string	"load_function"
.LC8:
	.string	"DOUBLE"
.LC9:
	.string	"name"
.LC10:
	.string	"value"
	.text
	.globl	marshal_create_class_object
	.type	marshal_create_class_object, @function
marshal_create_class_object:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movl	$.LC0, %edi
	call	create_base_obj_layout
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$nyxh_assign, %edx
	movl	$.LC1, %esi
	movq	%rax, %rdi
	call	add_class_object_function
	movq	-8(%rbp), %rax
	movl	$marshal_test, %edx
	movl	$.LC2, %esi
	movq	%rax, %rdi
	call	add_class_object_function
	movq	-8(%rbp), %rax
	movl	$marshal_open_module, %edx
	movl	$.LC3, %esi
	movq	%rax, %rdi
	call	add_class_object_function
	movq	-8(%rbp), %rax
	movl	$marshal_close_module, %edx
	movl	$.LC4, %esi
	movq	%rax, %rdi
	call	add_class_object_function
	movq	-8(%rbp), %rax
	movl	$marshal_open_library, %edx
	movl	$.LC5, %esi
	movq	%rax, %rdi
	call	add_class_object_function
	movq	-8(%rbp), %rax
	movl	$marshal_close_library, %edx
	movl	$.LC6, %esi
	movq	%rax, %rdi
	call	add_class_object_function
	movq	-8(%rbp), %rax
	movl	$marshal_load_function, %edx
	movl	$.LC7, %esi
	movq	%rax, %rdi
	call	add_class_object_function
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	get_base_class
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	create_class_instance
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	inc_obj_refcount
	movq	-24(%rbp), %rax
	movl	$.LC8, %edx
	movl	$.LC9, %esi
	movq	%rax, %rdi
	call	set_obj_string
	movq	-24(%rbp), %rax
	movl	$1, %edx
	movl	$.LC10, %esi
	movq	%rax, %rdi
	call	set_obj_int
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	add_member
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	marshal_create_class_object, .-marshal_create_class_object
	.globl	marshal_binding_open
	.type	marshal_binding_open, @function
marshal_binding_open:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	get_modules
	movq	%rax, -8(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	get_block_class
	movq	%rax, -16(%rbp)
	movq	-8(%rbp), %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	marshal_bind
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rcx
	movq	-40(%rbp), %rax
	movl	$.LC0, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	create_proxy_object
	movq	%rax, -32(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	inc_obj_refcount
	movq	-32(%rbp), %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	add_member
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	inc_obj_refcount
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	marshal_binding_open, .-marshal_binding_open
	.globl	marshal_binding_close
	.type	marshal_binding_close, @function
marshal_binding_close:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	marshal_binding_close, .-marshal_binding_close
	.globl	marshal_bind
	.type	marshal_bind, @function
marshal_bind:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	marshal_create_class_object
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rdx
	movq	-32(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	add_member
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	inc_obj_refcount
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	marshal_bind, .-marshal_bind
	.globl	marshal_open_module
	.type	marshal_open_module, @function
marshal_open_module:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	%r8, -40(%rbp)
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	marshal_open_module, .-marshal_open_module
	.globl	marshal_close_module
	.type	marshal_close_module, @function
marshal_close_module:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	%r8, -40(%rbp)
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	marshal_close_module, .-marshal_close_module
	.section	.rodata
.LC11:
	.string	"marshal.lib_handle"
	.text
	.globl	marshal_open_library
	.type	marshal_open_library, @function
marshal_open_library:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movq	%rdi, -56(%rbp)
	movq	%rsi, -64(%rbp)
	movq	%rdx, -72(%rbp)
	movq	%rcx, -80(%rbp)
	movq	%r8, -88(%rbp)
	movq	-88(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	node_GetItem
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	get_value
	movq	%rax, -24(%rbp)
	movq	$0, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	node_GetString
	movl	$1, %esi
	movq	%rax, %rdi
	call	dlopen
	movq	%rax, -32(%rbp)
	cmpq	$0, -32(%rbp)
	jne	.L10
	movq	-56(%rbp), %rax
	movq	%rax, %rdi
	call	get_false_class
	movq	%rax, -8(%rbp)
	jmp	.L11
.L10:
	movq	-56(%rbp), %rax
	movq	%rax, %rdi
	call	get_base_class
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	create_class_instance
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$.LC11, %edx
	movl	$.LC9, %esi
	movq	%rax, %rdi
	call	set_obj_string
	movq	-8(%rbp), %rdx
	movq	-56(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	add_garbage
	movq	-32(%rbp), %rdx
	movq	-8(%rbp), %rax
	movl	$.LC10, %esi
	movq	%rax, %rdi
	call	set_obj_ptr
.L11:
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	marshal_open_library, .-marshal_open_library
	.globl	marshal_close_library
	.type	marshal_close_library, @function
marshal_close_library:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	%rcx, -48(%rbp)
	movq	%r8, -56(%rbp)
	movq	-56(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	node_GetItem
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	get_value
	movq	%rax, %rdi
	call	node_GetValue
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	dlclose
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	get_true_class
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	marshal_close_library, .-marshal_close_library
	.section	.rodata
.LC12:
	.string	"privates"
.LC13:
	.string	"marshal_sym"
.LC14:
	.string	"return_type"
.LC15:
	.string	"marshal.return_value"
	.text
	.globl	marshal_call_function
	.type	marshal_call_function, @function
marshal_call_function:
.LFB10:
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
	call	node_GetItemsNum
	movq	%rax, -88(%rbp)
	movq	-240(%rbp), %rax
	movl	$.LC12, %esi
	movq	%rax, %rdi
	call	node_GetItemByKey
	movq	%rax, -96(%rbp)
	movq	-96(%rbp), %rax
	movl	$.LC13, %esi
	movq	%rax, %rdi
	call	node_GetItemByKey
	movq	%rax, -104(%rbp)
	movq	-240(%rbp), %rax
	movl	$.LC14, %esi
	movq	%rax, %rdi
	call	get_member
	movq	%rax, -112(%rbp)
	movq	$0, -24(%rbp)
	cmpq	$0, -112(%rbp)
	je	.L16
	movq	-112(%rbp), %rax
	movq	%rax, %rdi
	call	resolve_object
	movq	%rax, %rdi
	call	get_value
	movq	%rax, %rdi
	call	node_GetSint32
	movq	%rax, -24(%rbp)
.L16:
	movq	-104(%rbp), %rax
	movq	%rax, %rdi
	call	node_GetValue
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
	call	malloc
	movq	%rax, -160(%rbp)
	movq	-88(%rbp), %rax
	salq	$3, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -168(%rbp)
	movq	-264(%rbp), %rax
	movq	%rax, %rdi
	call	node_ItemReverseIterationReset
	jmp	.L17
.L20:
	movq	-264(%rbp), %rax
	movq	%rax, %rdi
	call	node_ItemReverseIterate
	movq	%rax, -176(%rbp)
	movq	-176(%rbp), %rax
	movq	%rax, %rdi
	call	get_value
	movq	%rax, -184(%rbp)
	movq	-184(%rbp), %rax
	movq	%rax, %rdi
	call	node_GetUint64
	movq	%rax, -192(%rbp)
	movq	-184(%rbp), %rax
	movq	%rax, %rdi
	call	node_GetType
	cmpb	$3, %al
	jne	.L18
	movq	-56(%rbp), %rdx
	movq	-160(%rbp), %rax
	addq	%rdx, %rax
	movb	$1, (%rax)
	addq	$1, -64(%rbp)
	jmp	.L19
.L18:
	movq	-56(%rbp), %rdx
	movq	-160(%rbp), %rax
	addq	%rdx, %rax
	movb	$0, (%rax)
.L19:
	movq	-56(%rbp), %rax
	leaq	0(,%rax,8), %rdx
	movq	-168(%rbp), %rax
	addq	%rax, %rdx
	movq	-192(%rbp), %rax
	movq	%rax, (%rdx)
	addq	$1, -56(%rbp)
.L17:
	movq	-264(%rbp), %rax
	movq	%rax, %rdi
	call	node_ItemReverseIterationUnfinished
	testl	%eax, %eax
	jne	.L20
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
	jns	.L21
	movq	$0, -72(%rbp)
.L21:
	cmpq	$0, -80(%rbp)
	jns	.L22
	movq	$0, -80(%rbp)
.L22:
	movq	-80(%rbp), %rdx
	movq	-72(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -152(%rbp)
#APP
# 553 "marshal.c" 1
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
	jne	.L23
#APP
# 566 "marshal.c" 1
	subq $8,%rsp
	
# 0 "" 2
#NO_APP
.L23:
	jmp	.L24
.L43:
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
	je	.L25
	cmpq	$0, -32(%rbp)
	jne	.L26
	movq	-200(%rbp), %rax
#APP
# 574 "marshal.c" 1
	movq %rax, %xmm0
	
# 0 "" 2
#NO_APP
	jmp	.L27
.L26:
	cmpq	$1, -32(%rbp)
	jne	.L28
	movq	-200(%rbp), %rax
#APP
# 576 "marshal.c" 1
	movq %rax, %xmm1
	
# 0 "" 2
#NO_APP
	jmp	.L27
.L28:
	cmpq	$2, -32(%rbp)
	jne	.L29
	movq	-200(%rbp), %rax
#APP
# 578 "marshal.c" 1
	movq %rax, %xmm2
	
# 0 "" 2
#NO_APP
	jmp	.L27
.L29:
	cmpq	$3, -32(%rbp)
	jne	.L30
	movq	-200(%rbp), %rax
#APP
# 580 "marshal.c" 1
	movq %rax, %xmm3
	
# 0 "" 2
#NO_APP
	jmp	.L27
.L30:
	cmpq	$4, -32(%rbp)
	jne	.L31
	movq	-200(%rbp), %rax
#APP
# 582 "marshal.c" 1
	movq %rax, %xmm4
	
# 0 "" 2
#NO_APP
	jmp	.L27
.L31:
	cmpq	$5, -32(%rbp)
	jne	.L32
	movq	-200(%rbp), %rax
#APP
# 584 "marshal.c" 1
	movq %rax, %xmm5
	
# 0 "" 2
#NO_APP
	jmp	.L27
.L32:
	cmpq	$6, -32(%rbp)
	jne	.L33
	movq	-200(%rbp), %rax
#APP
# 586 "marshal.c" 1
	movq %rax, %xmm6
	
# 0 "" 2
#NO_APP
	jmp	.L27
.L33:
	cmpq	$7, -32(%rbp)
	jne	.L34
	movq	-200(%rbp), %rax
#APP
# 588 "marshal.c" 1
	movq %rax, %xmm7
	
# 0 "" 2
#NO_APP
	jmp	.L27
.L34:
	movq	-200(%rbp), %rax
#APP
# 590 "marshal.c" 1
	pushq %rax
	
# 0 "" 2
#NO_APP
.L27:
	subq	$1, -32(%rbp)
	jmp	.L35
.L25:
	cmpq	$0, -40(%rbp)
	jne	.L36
	movq	-200(%rbp), %rax
#APP
# 596 "marshal.c" 1
	movq %rax,%rdi
	
# 0 "" 2
#NO_APP
	jmp	.L37
.L36:
	cmpq	$1, -40(%rbp)
	jne	.L38
	movq	-200(%rbp), %rax
#APP
# 598 "marshal.c" 1
	movq %rax,%rsi
	
# 0 "" 2
#NO_APP
	jmp	.L37
.L38:
	cmpq	$2, -40(%rbp)
	jne	.L39
	movq	-200(%rbp), %rax
#APP
# 600 "marshal.c" 1
	movq %rax,%rdx
	
# 0 "" 2
#NO_APP
	jmp	.L37
.L39:
	cmpq	$3, -40(%rbp)
	jne	.L40
	movq	-200(%rbp), %rax
#APP
# 602 "marshal.c" 1
	movq %rax,%rcx
	
# 0 "" 2
#NO_APP
	jmp	.L37
.L40:
	cmpq	$4, -40(%rbp)
	jne	.L41
	movq	-200(%rbp), %rax
#APP
# 604 "marshal.c" 1
	movq %rax,%r8
	
# 0 "" 2
#NO_APP
	jmp	.L37
.L41:
	cmpq	$5, -40(%rbp)
	jne	.L42
	movq	-200(%rbp), %rax
#APP
# 606 "marshal.c" 1
	movq %rax,%r9
	
# 0 "" 2
#NO_APP
	jmp	.L37
.L42:
	movq	-200(%rbp), %rax
#APP
# 610 "marshal.c" 1
	pushq %rax
	
# 0 "" 2
#NO_APP
.L37:
	subq	$1, -40(%rbp)
.L35:
	addq	$1, -48(%rbp)
.L24:
	movq	-48(%rbp), %rax
	cmpq	-88(%rbp), %rax
	jb	.L43
	cmpq	$8, -64(%rbp)
	jle	.L44
	movq	$8, -64(%rbp)
.L44:
	movq	-64(%rbp), %rax
	movq	-120(%rbp), %rbx
	#movq	%rcx, %rbx
#APP
# 620 "marshal.c" 1
	#movq %rax, %rax
	call %rbx
	#movq %rax,%rax
	
# 0 "" 2
#NO_APP
	movq	%rax, -128(%rbp)
	movq	-136(%rbp), %rax
#APP
# 629 "marshal.c" 1
	movq %rax,%rsp
	
# 0 "" 2
#NO_APP
	movq	-232(%rbp), %rax
	movq	%rax, %rdi
	call	get_base_class
	movq	%rax, -208(%rbp)
	movq	-208(%rbp), %rax
	movq	%rax, %rdi
	call	create_class_instance
	movq	%rax, -216(%rbp)
	movq	-216(%rbp), %rax
	movl	$.LC15, %edx
	movl	$.LC9, %esi
	movq	%rax, %rdi
	call	set_obj_string
	movq	-216(%rbp), %rdx
	movq	-232(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	add_garbage
	movq	-216(%rbp), %rax
	movq	%rax, %rdi
	call	get_value
	movq	%rax, -224(%rbp)
	movq	-128(%rbp), %rdx
	movq	-224(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	node_SetUint64
	movq	-224(%rbp), %rax
	movl	$10, %esi
	movq	%rax, %rdi
	call	node_SetType
	cmpq	$1, -24(%rbp)
	jne	.L45
	movq	-224(%rbp), %rax
	movl	$3, %esi
	movq	%rax, %rdi
	call	node_SetType
.L45:
	movq	-160(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movq	-168(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movq	-216(%rbp), %rax
	addq	$264, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	marshal_call_function, .-marshal_call_function
	.section	.rodata
.LC16:
	.string	"function %s not found\n"
.LC17:
	.string	"function"
.LC18:
	.string	"type"
.LC19:
	.string	"handler"
	.text
	.globl	marshal_load_function
	.type	marshal_load_function, @function
marshal_load_function:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$112, %rsp
	movq	%rdi, -72(%rbp)
	movq	%rsi, -80(%rbp)
	movq	%rdx, -88(%rbp)
	movq	%rcx, -96(%rbp)
	movq	%r8, -104(%rbp)
	movq	-104(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	node_GetItem
	movq	%rax, -8(%rbp)
	movq	-104(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	node_GetItem
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	get_value
	movq	%rax, -24(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	get_value
	movq	%rax, %rdi
	call	node_GetValue
	movq	%rax, -32(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	node_GetString
	movq	%rax, %rdx
	movq	-32(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	dlsym
	movq	%rax, -40(%rbp)
	cmpq	$0, -40(%rbp)
	jne	.L48
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	node_GetString
	movq	%rax, %rsi
	movl	$.LC16, %edi
	movl	$0, %eax
	call	printf
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	get_false_class
	jmp	.L49
.L48:
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	get_base_class
	movq	%rax, -48(%rbp)
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	create_class_instance
	movq	%rax, -56(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	node_GetString
	movq	%rax, %rdx
	movq	-56(%rbp), %rax
	movl	$.LC9, %esi
	movq	%rax, %rdi
	call	set_obj_string
	movq	-56(%rbp), %rax
	movl	$.LC17, %edx
	movl	$.LC18, %esi
	movq	%rax, %rdi
	call	set_obj_string
	movq	-56(%rbp), %rax
	movl	$.LC12, %esi
	movq	%rax, %rdi
	call	node_GetItemByKey
	movq	%rax, -64(%rbp)
	movq	-64(%rbp), %rax
	movl	$marshal_call_function, %edx
	movl	$.LC19, %esi
	movq	%rax, %rdi
	call	set_obj_ptr
	movq	-40(%rbp), %rdx
	movq	-64(%rbp), %rax
	movl	$.LC13, %esi
	movq	%rax, %rdi
	call	set_obj_ptr
	movq	-56(%rbp), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	add_garbage
	movq	-56(%rbp), %rax
.L49:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	marshal_load_function, .-marshal_load_function
	.section	.rodata
.LC20:
	.string	"marshal.test"
.LC21:
	.string	"marshalling test"
.LC22:
	.string	"libc.so.6"
.LC23:
	.string	"library not found"
.LC24:
	.string	"checking for printf call"
.LC25:
	.string	"printf"
.LC26:
	.string	"no printf function found"
.LC27:
	.string	"trying printf call"
.LC28:
	.string	"printf[%s]\n"
.LC29:
	.string	"hello world"
.LC30:
	.string	"call finished"
	.text
	.globl	marshal_test
	.type	marshal_test, @function
marshal_test:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	addq	$-128, %rsp
	movq	%rdi, -88(%rbp)
	movq	%rsi, -96(%rbp)
	movq	%rdx, -104(%rbp)
	movq	%rcx, -112(%rbp)
	movq	%r8, -120(%rbp)
	movq	-88(%rbp), %rax
	movq	%rax, %rdi
	call	get_base_class
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	create_class_instance
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movl	$.LC20, %edx
	movl	$.LC9, %esi
	movq	%rax, %rdi
	call	set_obj_string
	movq	-16(%rbp), %rdx
	movq	-88(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	add_garbage
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	get_value
	movq	%rax, -24(%rbp)
	movl	$.LC21, %edi
	call	puts
	movl	$1, %esi
	movl	$.LC22, %edi
	call	dlopen
	movq	%rax, -32(%rbp)
	cmpq	$0, -32(%rbp)
	jne	.L51
	movl	$.LC23, %edi
	call	puts
	jmp	.L52
.L51:
	movl	$.LC24, %edi
	call	puts
	movq	-32(%rbp), %rax
	movl	$.LC25, %esi
	movq	%rax, %rdi
	call	dlsym
	movq	%rax, -40(%rbp)
	cmpq	$0, -40(%rbp)
	jne	.L53
	movl	$.LC26, %edi
	call	puts
	jmp	.L54
.L53:
	movl	$.LC27, %edi
	call	puts
	movq	-40(%rbp), %rax
	movq	%rax, -48(%rbp)
	movq	$.LC28, -56(%rbp)
	movq	$.LC29, -64(%rbp)
	movq	$0, -72(%rbp)
	movq	-64(%rbp), %rax
	movq	-56(%rbp), %rdx
	movq	-40(%rbp), %rcx
#APP
# 1225 "marshal.c" 1
	movq %rdx,%rdi
	movq %rax,%rsi
	movl $0,%eax
	call %rcx
	movq %rax,%rax
	
# 0 "" 2
#NO_APP
	movq	%rax, -72(%rbp)
	movl	$.LC30, %edi
	call	puts
	movq	stdout(%rip), %rax
	movq	%rax, %rdi
	call	fflush
.L54:
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	dlclose
.L52:
	movq	-16(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	marshal_test, .-marshal_test
	.ident	"GCC: (Debian 4.9.1-15) 4.9.1"
	.section	.note.GNU-stack,"",@progbits
