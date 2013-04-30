#include <boost/program_options.hpp>
#include <iostream>
#include <stdio.h>
#include <sys/time.h>

double inline now()
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_usec / 1000000.0 + t.tv_sec;
}

int main(int argc, char **argv)
{
	/* Initialize */
	long numFlopPerIteration = 1000;
	long numIterations = -1; /* i.e., infinity */
	volatile double a = 1.1;
	volatile double b = 1.1; /* we are going to operate on these numbers quite a few times */

	/* Parse command-line */
	namespace po = boost::program_options;

	po::options_description desc("Process performance statistics");
	desc.add_options()
		("help", "display this help message")
		("flop", po::value<long>(&numFlopPerIteration), "set number of floating-point operations per iteration (default: 1000)")
		("iterations", po::value<long>(&numIterations), "set number of iterations (default: infinity)")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	/* If requested, display help and exit */
	if (vm.count("help")) {
		std::cerr << desc << "\n";
		return 1;
	}

	/* Do operations and report statistics */
	/* NOTE: we make the assumptions that numFlopPerIteration is high enough
	 * to render all other computations / calls negligible */
	double lastReport = now();
	int numFlopSinceLastReport = 0;
	for (long i = 0; i < numIterations || numIterations == -1; i++) {
		for (long j = 0; j < numFlopPerIteration; j++) {
			a *= b;
		}
		numFlopSinceLastReport += numFlopPerIteration;

		/* Display statistics */
		double currentTime = now();
		double timeDifference = currentTime - lastReport;
		if (timeDifference >= 1) {
			printf("[%.06f] %f\n", currentTime, numFlopSinceLastReport / timeDifference);
			lastReport = currentTime;
			numFlopSinceLastReport = 0;
		}
	}

	/* Trick compiler */
	fprintf(stderr, "Residual: %f\n", a);

	return 0;
}

