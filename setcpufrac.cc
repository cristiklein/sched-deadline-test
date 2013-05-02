#include <boost/program_options.hpp>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/utsname.h>

extern "C" {
#include "dl_syscalls.h"
}

double inline now()
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_usec / 1000000.0 + t.tv_sec;
}

int main(int argc, char **argv)
{
	/* Initialize */
	pid_t pid = 0;
	float frac = 0.5; /* just for fun */

	/* Parse command-line */
	namespace po = boost::program_options;

	po::options_description desc("Use SCHED_DEADLINE to set the CPU fraction of a process");
	desc.add_options()
		("help", "display this help message")
		("pid", po::value<pid_t>(&pid), "process ID to act on")
		("frac", po::value<float>(&frac), "fraction of CPU to reserve (default: 0.5)")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	/* If requested, display help and exit */
	if (vm.count("help") || pid == 0) {
		std::cerr << desc << "\n";
		return 1;
	}

	/* Check ABI */
	struct utsname utsname;
	uname(&utsname); /* XXX: assume this never fails */
	if (std::string(utsname.release) != SCHED_DEADLINE_ABI_COMPATIBLE_WITH) {
		fprintf(stderr, "WARNING: This binary uses experimental kernel ABI. It has been tested with Linux %s, whereas you are currently using Linux %s. Please read the source code, both of this executable and the currently installed kernel and make sure the definitions match.\n", SCHED_DEADLINE_ABI_COMPATIBLE_WITH, utsname.release);
	}

	/* Call SCHED_DEADLINE API */
	struct sched_param2 param2;
	param2.sched_priority = 0;
	param2.sched_deadline = 100000;
	param2.sched_period = 100000;
	param2.sched_runtime = (uint64_t)floor(frac * param2.sched_period);
	int ret = sched_setscheduler2(pid, SCHED_DEADLINE, &param2);
	if (ret != 0) {
		int _errno = errno; /* save errno */
		fprintf(stderr, "Error in sched_setscheduler2: %s\n"
			"  pid %d\n"
			"  runtime  %llu\n"
			"  deadline %llu\n"
			"  period   %llu\n",
			strerror(_errno), pid, param2.sched_runtime, param2.sched_deadline, param2.sched_period);
	}

	return ret;
}

