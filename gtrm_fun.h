#ifndef _GTRM_FUNCTIONS
#define _GTRM_FUNCTIONS

  #include <errno.h>
  #include <sched.h>
  #include <signal.h>
  #include <stdlib.h>
  #include <jobsignaler.h>
  #include <unistd.h> 
  #include <sys/types.h>
  #include <math.h>

  #include "dl_syscalls.h"

  #define _DEBUG 1
  #define _EXIT_SUCCESS 0
  #define _RM_MAX_APPLICATIONS 100
  #define _RM_DEADLINE 1000000 // nsec
  #define _RM_RUNTIME    10000 // nsec
  #define _MIN_SINGLE_ASSIGNABLE 0.10
  #define _MAX_SINGLE_ASSIGNABLE 0.90
  #define _MAX_ASSIGNABLE (0.9000 * 1) //sysconf(_SC_NPROCESSORS_ONLN))

  #ifdef _DEBUG
  #define debug_info(M)         \
    ({                            \
       fprintf(stderr, "Info: "); \
       fprintf(stderr, M);      \
       fprintf(stderr, "\n");     \
    }) 
  #else
  #define debug_info(...)
  #endif // _DEBUG

  #define fatal(M)               \
    ({                             \
       fprintf(stderr, "Fatal: "); \
       fprintf(stderr, M);       \
       fprintf(stderr, "\n");      \
                                   \
       exit(EXIT_FAILURE);         \
    })

  #define unreachable() \
    fatal("unreachable");

  int apply_scheddeadline(pid_t tid, float vp) {
    struct sched_param2 param2;
    param2.sched_priority = 0;
    param2.sched_deadline = _RM_DEADLINE;
    param2.sched_period = _RM_DEADLINE;
    param2.sched_runtime = (uint64_t) floor(vp * param2.sched_period);
    pid_t rm_pid = getpid();
    int ret = sched_setscheduler2(tid, SCHED_DEADLINE, &param2);
    #ifdef _DEBUG
      if (ret != 0)
        fprintf(stderr, "Error in sched_setscheduler2 for pid %d, vp %f, runtime %llu, errno %d\n", tid, vp, param2.sched_runtime, errno);
    #endif
    return ret;
  }

#endif
