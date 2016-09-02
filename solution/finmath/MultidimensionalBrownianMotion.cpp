#include "stdafx.h"
#include "MultidimensionalBrownianMotion.h"

namespace Sample
{
ShareInfo::ShareInfo() {}
ShareInfo::ShareInfo(double S0, double mu, double sigma) : S0(S0), sigma(sigma), nu(mu-sigma*sigma/2) {};

ShareInfo::ShareInfo(const ShareInfo& info)
{
	this->operator=(info);
};

void ShareInfo::operator=(const ShareInfo& info)
{
	S0 = info.S0;
	sigma = info.sigma;
	nu = info.nu;
}

double ShareInfo::get_S0() const
{
	return S0;
}

double ShareInfo::get_sigma() const
{
	return sigma;
}

double ShareInfo::get_nu() const
{
	return nu;
}

Matrix MultidimensionalBrownianMotion::Simulate(const vector<ShareInfo>& shares, const vector<double>& Corr, int T, int steps, int sims)
{
	int numAssets = shares.size();
	double dt = 1.0/252;
	Matrix vPrices(steps*sims, numAssets);

	vector<double> nu(numAssets);
	vector<double> sigma(numAssets);

	for(int i=0; i<numAssets; ++i) {
		nu[i] = shares[i].get_nu();
		sigma[i] = shares[i].get_sigma();
	}

	//populate matrix of size steps by numAssets with nu*dt
	//Matrix nudt(steps*sims, vector<double>(numAssets));
	Matrix nudt(steps*sims, numAssets);

	for (int i = 0; i < steps*sims; ++i) {
		for (int j = 0; j < numAssets; ++j) {
			nudt(i,j) = nu[j]*dt;
		}
	}

	//convert vector sigma into diagonal matrix with zeros on off-diagonals
	Matrix diagSig(numAssets, numAssets);
	for(int i=0; i<numAssets; ++i) {
		for(int j=0; j<numAssets; ++j) {
			diagSig(i,j) = i == j ? sigma[i] : 0.0;
		}
	}

	//Multidimensional Brownian Motion Simulation Begins

	srand((unsigned)time(0));

	//Matrix randnMatrix(steps*sims, vector<double> (numAssets));
	Matrix randnMatrix(steps*sims, numAssets);

	NormalDistribution random_normal;

	for (int i = 0; i < steps*sims; i++) {
		for (int j = 0; j < numAssets; j++) {
			//populate random matrix
			randnMatrix(i,j) = random_normal.nextValue();
		}
	}

	//correlate the random variables
	//Matrix Z = matrixMult(randnMatrix, Y, steps*sims, numAssets, numAssets);
	//Matrix C = matrixMult(Z, diagSig, steps*sims, numAssets, numAssets);
	//Matrix vPrices(steps*sims, vector<double> (numAssets));

	Matrix Z = randnMatrix * CholeskyUpperTriangularMatrix<double>(Corr, numAssets);
	Matrix C = Z * diagSig;
//	Matrix vPrices(steps*sims, numAssets);

	for (int i = 0; i < steps*sims-1; i++) {
		for (int j = 0; j < numAssets; j++) {
			//initialize new paths with initial asset prices or generate full paths
			vPrices(i,j) = i==0 || (i+1)%252==0 ? shares[j].get_S0() : exp(nudt(i,j) + C(i,j)*sqrt(dt)) * vPrices(i-1,j);
		}
	}

	return vPrices;
}

}