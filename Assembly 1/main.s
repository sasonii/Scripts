/*207826215 Lior Sabban*/
.extern rand
.extern srand
.extern printf
.extern atoi

.section .rodata
prompt: .asciz "Enter configuration seed: "
guessPrompt: .asciz "What is your guess? "
successMsg: .asciz "Congratz! You won!\n"
failure: .asciz "Incorrect.\n"
lose: .asciz "Game over, you lost :(. The correct answer was %d\n"
scanfForm: .asciz "%d"

.section .data
.seed: .space 4
.guess: .space 4
random: .space 4

.section .text
.globl main
main:

    pushq %rbp
    movq %rsp, %rbp

    # Get the seed for rand function from the user
    movl $4, %eax
    movl $1, %ebx
    leal prompt, %ecx
    movl $26, %edx
    int $0x80
	
	movq    $scanfForm, %rdi
	 leaq    .seed, %rsi
	 xorq    %rax, %rax
	 call    scanf
	 
    # Read the seed from stdin
    
   


    /*
    #push %rbx
    movq $scanfForm, %rdi 
    mov .seed, %rsi
    xorq %rax, %rax 
    call scanf
    #pop %rbx
    #subl $0xa30, .seed
    */

    

    # Generate a random number between 0 and 9

    xorq %rdi, %rdi
    movl .seed, %edi 
    xorq %rax, %rax 
    call srand 

    xorq %rax, %rax
    call rand

    movl $10, %ecx
    xorl %edx, %edx 
    idiv %ecx 

    movl %edx, random

/*
    leal .seed, %edi
    xor %rax, %rax 
    call atoi
    */

    # Give the user 5 attempts to guess the number
    movq $5, %r12

guessLoop:
    # Prompt the user to guess a number
    
    movl $4, %eax
    movl $1, %ebx
    leal guessPrompt, %ecx
    movl $20, %edx
    int $0x80

    # Read the guess from stdin
    /*
    pushq %rbx
    movq $scanfForm, %rdi 
    lea .guess, %rsi
    xorq %rax, %rax 
    
    call scanf
    popq %rbx
    */
    
    movq    $scanfForm, %rdi
	 leaq    .guess, %rsi
	 xorq    %rax, %rax
	 pushq %r12
	 pushq %r12
	 call    scanf
	popq %r12
	popq %r12
    

    # Check if the guess is correct
    movl .guess, %eax
    cmpl random, %eax
    je success

    # Decrement the attempt count and loop if not zero
    # Output failure message
    movl $4, %eax
    movl $1, %ebx
    leal failure, %ecx
    movl $11, %edx
    int $0x80

    decq %r12
    jnz guessLoop

    # Output failure message
    # Print the random number
/*
    push %rbx
    movl .random, %esi       
    lea lose(%rip), %rdi
    xor %rax, %rax
    call printf
    pop %rbx

    jmp end
    */

    #push %rbp
    movq $lose, %rdi 
    movq random, %rsi 
    xorq %rax, %rax 
    call printf
    #pop %rbp 
    jmp end

success:
    # Output success message
    movl $4, %eax
    movl $1, %ebx
    leal successMsg, %ecx
    movl $19, %edx
    int $0x80

end:
    # Exit
    /*
    movl $1, %eax
    xorl %ebx, %ebx
    int $0x80
    */

    xorq %rax, %rax 
    movq %rbp, %rsp
    popq %rbp
    ret
    
