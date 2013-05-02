#include <boost/program_options.hpp>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

#include "doMath.hh"

double inline now()
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_usec / 1000000.0 + t.tv_sec;
}


int main(int argc, char **argv)
{
	/* Initialize */
	long numIterations = 1000;
	double residual = 1.1;

	/* Parse command-line */
	namespace po = boost::program_options;

	po::options_description desc("Process performance statistics");
	desc.add_options()
		("help", "display this help message")
		("iterations", po::value<long>(&numIterations), "set number of iterations (default: 1000)")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	/* If requested, display help and exit */
	if (vm.count("help")) {
		std::cerr << desc << "\n";
		return 1;
	}

	/* Print our PID for control purposes */
	fprintf(stderr, "My PID: %d\n", getpid());

	/* Do operations and report statistics */
	/* NOTE: we make the assumptions that numFlopPerIteration is high enough
	 * to render all other computations / calls negligible */
	double lastReport = now();
	uint64_t numFlopSinceLastReport = 0;
	for (int i = 0; i < 10000000 /* avoid optimizations */; i++) {
		residual += test_dp_mac_SSE(numIterations);
		numFlopSinceLastReport += 48 * 1000 * numIterations * 2;

		/* Display statistics */
		double currentTime = now();
		double timeDifference = currentTime - lastReport;
		if (timeDifference >= 1) {
			printf("[%.06f] %.2f GFLOPS\n", currentTime, numFlopSinceLastReport / timeDifference / 1000000000);
			lastReport = currentTime;
			numFlopSinceLastReport = 0;
		}
	}

	/* Trick compiler */
	fprintf(stderr, "Residual: %f\n", residual);

	return 0;
}

