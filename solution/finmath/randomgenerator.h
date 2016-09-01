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
public: 
	NormalDistribution();
	NormalDistribution(double mean, double stdev);
	~NormalDistribution();
	
	double nextValue();

	std::default_random_engine generator_;
	std::normal_distribution<double> distribution_;

};

}