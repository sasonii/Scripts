.extern printf, pstrlen, swapCase, pstrijcpy, scanf
.text
    .global run_func
    .type run_func, @function
# rdi - choice integer
#   %esi - pointer to first Pstring
#   %edx - pointer to second Pstring
run_func:
    # Save registers
    pushq    %rbp
    movq    %rsp, %rbp
    pushq %rbx
    # Check the choice integer
    cmp $31, %edi
    je calculate_lengths
    cmp $33, %edi
    je swap_case
    cmp $34, %edi
    je copy_strings
    jmp invalid_option
calculate_lengths:
    # Call pstrlen to calculate the length of the first Pstring
    movq %rsi, %rdi
    call pstrlen
    movl %eax, %ecx
    # ecx holds the length of the first Pstring
    # Call pstrlen to calculate the length of the second Pstring
    movq %rdx, %rdi
    xorq %rax, %rax
    call pstrlen
    movl %eax, %edx
    # edx holds the length of the second Pstring
    push %rbx 
	movq    $format_lengths, %rdi
    movq    %rcx, %rsi
    movq    %rdx, %rdx 
    xorq    %rax, %rax
    call    printf
    pop %rbx
    jmp end_func
swap_case:
# save the address of the second Pstring in rdx  
	pushq    %rdx
    # move string as parameter to swapCase function
    movq    %rsi, %rdi
    call    swapCase
    movq    $format_string, %rdi
    # rax currently holds the address of the first Pstring that came from swapCase function
    movzbq    (%rax), %rsi
    # increment rax to point to the first character of the string
    incq    %rax
    # save the address of the second Pstring in rdx  
	# pushq    %rdx

    movq    %rax, %rdx
    
    xorq    %rax, %rax
    call    printf
    
    # move second string as parameter to swapCase function
    popq    %rdi

    # mov %rdx, %rdi

    call    swapCase

    push %rbx

    movq    $format_string, %rdi
    movzbq  (%rax), %rsi
    incq    %rax
    movq    %rax, %rdx
    xorq    %rax, %rax
    call    printf

    pop %rbx 

    jmp end_func

copy_strings:
    pushq    %rbp
	movq    %rsp, %rbp
	subq    $16, %rsp
	pushq    %rdx
	pushq    %rsi
	 movq    $format_input, %rdi
	 leaq    16(%rsp), %rsi
	 xorq    %rax, %rax
	 call    scanf
	 movq    $format_input, %rdi
	 leaq    20(%rsp), %rsi
	 xorq    %rax, %rax
	 call    scanf
    # first string
	popq    %rdi
    # save for later
	movq    %rdi, %r12
    # second string
	popq    %rsi
    # save the string for later
	movq    %rsi, %r13
	movl    (%rsp), %edx
	movl    4(%rsp), %ecx
    # movl    $1, %edx
	# movl    $3, %ecx
    pushq    %r12
    pushq    %r13
	call    pstrijcpy
    popq    %r13   
    popq    %r12
    # print the first string
	movq    $format_string, %rdi
    # length of the first string
	movzbq    (%r12), %rsi
    # increment r12 to point to the first character of the string
	incq    %r12
    # move as parameter to printf
	movq    %r12, %rdx
	xorq    %rax, %rax
	call    printf
	movq    $format_string, %rdi
	movzbq    (%r13), %rsi
	incq    %r13
	movq    %r13, %rdx
	xorq    %rax, %rax
	call    printf
	movq    %rbp, %rsp
	popq    %rbp
    jmp end_func
invalid_option:
    # Print the error message for invalid option
    
    movq    $format_invalid, %rdi
    xorq    %rax, %rax
    call printf
   
/*
    movl $4, %eax
    movl $1, %ebx 
    leal format_invalid, %ecx 
    movl $17, %edx 
    int $0x80 
 */
end_func:
    # Restore registers
    popq %rbx
    # Return from the function
    movq    %rbp, %rsp
    popq    %rbp
    ret
# Format strings
format_lengths: .asciz "first pstring length: %d, second pstring length: %d\n"
format_string: .asciz "length: %d, string: %s\n"
format_start: .asciz "Enter the start index: "
format_input: .asciz "%d"
format_invalid: .asciz "invalid option!\n"
