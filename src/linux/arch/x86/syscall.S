        .text
ENTRY (syscall)
        PUSHARGS_6                /* Save register contents.  */
        _DOARGS_6(44)                /* Load arguments.  */
        movl 20(%esp), %eax        /* Load syscall number into %eax.  */
        ENTER_KERNEL                /* Do the system call.  */
        POPARGS_6                /* Restore register contents.  */
        cmpl $-4095, %eax        /* Check %eax for error.  */
        jae SYSCALL_ERROR_LABEL        /* Jump to error handler if error.  */
        ret                        /* Return to caller.  */
PSEUDO_END (syscall)
