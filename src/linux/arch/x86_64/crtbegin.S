.global _start             
_start:                     
    movq 0(%rsp), %rdi        
    leaq 8(%rsp), %rsi        
    leaq 8(%rsi,%rdi,8), %rdx 
    movq %rsp, %rbp 
    call main 
    movq %rax, %rdi 
    movl $231, %eax 
    syscall
