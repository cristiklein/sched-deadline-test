Test program for SCHED\_DEADLINE
===============================

This repository contains two programs:

* a single-threaded FPU burner, which reports every second the FLOPS that it has achieved;
* a utility that uses [SCHED\_DEADLINE](https://github.com/jlelli/sched-deadline "SCHED_DEADLINE") to control the fraction of the CPU to allocate to a process.

The two can be used to test SCHED\_DEADLINE as follows. In one terminal run `./processperfstat` and observe the actived FLOPS. In another terminal run `./setprocfrac --pid PID --frac 0.5`. The FLOPS should be reduced by the amount specified as `FRAC`.

Compilation
-----------

Prerequisites:

* a Linux kernel patched with SCHED\_DEADLINE support ([git](https://github.com/jlelli/sched-deadline "SCHED_DEADLINE"));
* libboost_program_options.

Compilation:

* `make`

**Important Note**
------------------
**The utility uses experimental kernel ABI.** Before compiling it, make sure that the syscall numbers in `dl_syscalls.h` match the ones in the kernel you use! Also note that syscall numbers differ between architectures. The provided file was tested with commit ID e198ff46022a806dbaa9208fbdc2247266da1d11 on an x86\_64. For other source versions / architectures you **must adjust** `dl_syscalls.h`.

