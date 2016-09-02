#pragma once
#include <random>

namespace Sample {

class RandomGenerator
{
	public:
		virtual double nextValue() = 0;
};


class NormalDistribution : public RandomGenerator
{
	std::default_random_engine _generator;
	std::normal_distribution<double> _distribution;

public: 
	NormalDistribution();
	NormalDistribution(double mean, double stdev);
	
	double nextValue();

};

}