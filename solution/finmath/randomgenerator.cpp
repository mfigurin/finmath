#include "stdafx.h"
#include "randomgenerator.h"
#include "random"
namespace Sample {

// mean - Mean of the distribution(its expected value, μ).Which coincides with the location of its peak.
// stddev -Standard deviation(σ) : The square root of variance, representing the dispersion of values from the distribution mean.
// This shall be a positive value(σ>0).

NormalDistribution::NormalDistribution(double mean, double stdev)
{
	
	// See http://www.cplusplus.com/reference/random/normal_distribution/
	std::default_random_engine generator;
	//std::mt19937 generator(rd());
	std::normal_distribution<double> distribution(mean, stdev);
	generator_ = generator;
	distribution_ = distribution;
}

NormalDistribution::NormalDistribution() {
	NormalDistribution(0.0, 1.0);
}


NormalDistribution::~NormalDistribution()
{
}

double NormalDistribution::nextValue() {
	return distribution_(generator_);
}

}
