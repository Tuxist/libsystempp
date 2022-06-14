/*******************************************************************************
Copyright (c) 2022, Jan Koester jan.koester@gmx.net
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include "syscall.h"
#include "sysbits.h"

#include "systempp/sysexec.h"
#include "systempp/sysexception.h"

#define P_ALL   0
#define P_PID   1
#define P_PGID  2
#define P_PIDFD 3

#define WNOHANG     0x00000001
#define WUNTRACED   0x00000002
#define WSTOPPED    WUNTRACED
#define WEXITED     0x00000004
#define WCONTINUED  0x00000008
#define WNOWAIT     0x01000000

# define __SI_MAX_SIZE     128
# if __WORDSIZE == 64
#  define __SI_PAD_SIZE     ((__SI_MAX_SIZE / sizeof (int)) - 4)
# else
#  define __SI_PAD_SIZE     ((__SI_MAX_SIZE / sizeof (int)) - 3)
# endif

typedef int __pid_t;
typedef int __uid_t;
typedef long int time_t;
typedef signed long __clock_t;

typedef union sigval {
    int sival_int;
    void *sival_ptr;
} sigval_t;

typedef struct siginfo {
    int si_signo;                /* Signal number.  */
    int si_errno;                /* If non-zero, an errno value associated with
                                   this signal, as defined in <errno.h>.  */
    int si_code;                /* Signal code.  */

    union {
        int _pad[__SI_PAD_SIZE];
        struct {
            __pid_t si_pid;        /* Sending process ID.  */
            __uid_t si_uid;        /* Real user ID of sending process.  */
          } _kill;
        struct {
            int si_tid;                /* Timer ID.  */
            int si_overrun;        /* Overrun count.  */
            sigval_t si_sigval;        /* Signal value.  */
          } _timer;

        struct{
            __pid_t si_pid;        /* Sending process ID.  */
            __uid_t si_uid;        /* Real user ID of sending process.  */
            sigval_t si_sigval;        /* Signal value.  */
          } _rt;
        struct {
            __pid_t si_pid;        /* Which child.  */
            __uid_t si_uid;        /* Real user ID of sending process.  */
            int si_status;        /* Exit value or signal.  */
            __clock_t si_utime;
            __clock_t si_stime;
        } _sigchld;
        struct{
            void *si_addr;        /* Faulting insn/memory ref.  */
        } _sigfault;
        struct{
            long int si_band;        /* Band event for SIGPOLL.  */
            int si_fd;
          } _sigpoll;
      } _sifields;
} siginfo_t;

struct timeval {
   time_t      tv_sec;   // Number of whole seconds of elapsed time
   long int    tv_usec;  // Number of microseconds of rest of elapsed time minus tv_sec. Always less than one million
};

struct  rusage {
    struct timeval ru_utime;    /* user time used */
    struct timeval ru_stime;    /* system time used */
    long    ru_maxrss;      /* maximum resident set size */
    long    ru_ixrss;       /* integral shared memory size */
    long    ru_idrss;       /* integral unshared data size */
    long    ru_isrss;       /* integral unshared stack size */
    long    ru_minflt;      /* page reclaims */
    long    ru_majflt;      /* page faults */
    long    ru_nswap;       /* swaps */
    long    ru_inblock;     /* block input operations */
    long    ru_oublock;     /* block output operations */
    long    ru_msgsnd;      /* messages sent */
    long    ru_msgrcv;      /* messages received */
    long    ru_nsignals;        /* signals received */
    long    ru_nvcsw;       /* voluntary context switches */
    long    ru_nivcsw;      /* involuntary " */
};

sys::Exec::Exec(const char *wrkdir[],const char *filename,const char *argv[]){
    SystemException exception;
    _Pid=syscall0(__NR_fork);
    if(_Pid <0)
        exception[SystemException::Error] << "Exec fork failed !";
    else if(_Pid==0)
        syscall3(__NR_execve,(unsigned long)filename,(unsigned long)argv,(unsigned long)wrkdir);
}

sys::Exec::~Exec(){
}

int sys::Exec::join(){
    struct siginfo sinfo;
    struct rusage rusage;
    if(_Pid!=0)
        return syscall5(__NR_waitid,P_PID,_Pid,(unsigned long)&sinfo,WEXITED,(unsigned long) &rusage);
    return 0;
}
