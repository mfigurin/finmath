#pragma once

#include<iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "Matrix.h"

using namespace std;

namespace Sample {

class Share
{
	public:
		const double S0;
		const double mu;
		const double sigma;
		const double nu;

		Share(double S0, double mu, double sigma) : S0(S0), mu(mu), sigma(sigma), nu(mu-sigma*sigma/2) {};
	};

	class MultidimensionalBrownianMotion
	{
	public:
		MultidimensionalBrownianMotion(void);
		~MultidimensionalBrownianMotion(void);

		//S0: initial price vector
		//mu: mean vector
		//sigma: standard deviation vector
		//Corr: correlation matrix
		//T: time (in years)
		//numAssets: number of assets
		//steps: number of days
		//sims: number of iterations
		Matrix<double> multiBrownianMotion(
			vector<Share> shares,
			vector<double> Corr,
			int T,
			int steps,
			int sims)
		{
			int numAssets = shares.size();
			double dt = 1.0/252;

			vector<double> nu = vector<double>(numAssets);
			vector<double> sigma = vector<double>(numAssets);

			for(int i=0; i<numAssets; ++i) {
				nu[i] = shares[i].nu;
				sigma[i] = shares[i].sigma;
			}

			//Cholesky decomposition of correlation matrix to produce upper triangular matrix
			//Matrix Y = transpose(cholesky(Corr), numAssets, numAssets);
			CholeskyUpperTriangularMatrix<double> Y = CholeskyUpperTriangularMatrix<double>(Corr, numAssets);

			//populate matrix of size steps by numAssets with nu*dt
			//Matrix nudt(steps*sims, vector<double>(numAssets));
			Matrix<double> nudt = Matrix<double>(steps*sims, numAssets);

			for (int i = 0; i < steps*sims; ++i) {
				for (int j = 0; j < numAssets; ++j) {
					nudt(i,j) = nu[j]*dt;
				}
			}

			//convert vector sigma into diagonal matrix with zeros on off-diagonals
			Matrix diagSig = diag(sigma, numAssets);

			//Multidimensional Brownian Motion Simulation Begins

			srand((unsigned)time(0));

			//Matrix randnMatrix(steps*sims, vector<double> (numAssets));
			Matrix<double> randnMatrix(steps*sims, numAssets);

			for (int i = 0; i < steps*sims; i++) {
				for (int j = 0; j < numAssets; j++) {
					//populate random matrix
					randnMatrix(i,j) = random_normal();
				}
			}

			//correlate the random variables
			//Matrix Z = matrixMult(randnMatrix, Y, steps*sims, numAssets, numAssets);
			//Matrix C = matrixMult(Z, diagSig, steps*sims, numAssets, numAssets);
			//Matrix vPrices(steps*sims, vector<double> (numAssets));

			Matrix<double> Z = randnMatrix * Y;
			Matrix<double> C = Z * diagSig;
			Matrix<double> vPrices = Matrix<double>(steps*sims, numAssets);

			for (int i = 0; i < steps*sims-1; i++) {
				for (int j = 0; j < numAssets; j++) {
					//initialize new paths with initial asset prices or generate full paths
					vPrices(i,j) = i==0 || (i+1)%252==0 ? shares[j].S0 : exp(nudt(i,j) + C(i,j)*sqrt(dt)) * vPrices(i-1,j);
				}
			}

			return vPrices;
		};

		//Takes in vector and writes its entires on the diagonal of a matrix with zeros elsewhere
		Matrix diag(vector<double> V, int n)
		{
			vector<vector<double> > ans(n, vector<double> (n));
			for (int j=0; j< n; j++)
			{
				for (int i=0; i<n; i++)
				{
					ans[i][j] = 0;
				}
			}

			for (int j=0; j< n; j++)
			{
				for (int i=0; i<n; i++)
				{
					ans[i][i] = V[i];
				}
			}
			return ans;
		}
	};

}
