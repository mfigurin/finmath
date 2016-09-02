#pragma once

#include<iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "Matrix.h"
#include "randomgenerator.h"
using namespace std;

namespace Sample {

class ShareInfo
{
private:
	double S0;
	double sigma;
	double nu;

public:
	ShareInfo();
	ShareInfo(double S0, double mu, double sigma);
	ShareInfo(const ShareInfo& info);

	void operator=(const ShareInfo& info);

	double get_S0() const;
	double get_sigma() const;
	double get_nu() const;
};

class MultidimensionalBrownianMotion
{
public:
	//Corr: correlation matrix
	//T: time (in years)
	//steps: number of days
	//sims: number of iterations
	static Matrix Simulate(const vector<ShareInfo>& shares, const vector<double>& Corr, int T, int steps, int sims);
};

}
