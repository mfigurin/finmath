#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../finmath/Matrix.h"

namespace FinmathUnitTest
{
	TEST_CLASS(MatrixTest)
	{
	public:

		TEST_METHOD(MatrixTest1)
		{

			Sample::Matrix<double> matrix1(1,3,1);
			Sample::Matrix<double> matrix2(3,3,1);
			Sample::Matrix<double> result = matrix1*matrix2;

			Assert::AreEqual( result.rows(), 1);
			Assert::AreEqual( result.cols(), 3);

			for( int i  = 0; i < result.rows(); i++ ) {
				for( int j = 0; j < result.cols(); j++ ) {
					Assert::AreEqual( result(i,j), 3.0);
				}
			}
		}


		TEST_METHOD(CholeskyTestSampleInput)
		{
			static const int SHARES_COUNT = 3;
			
			std::vector<double> correlations(SHARES_COUNT);
			correlations[0] = 0.35;
			correlations[1] = -0.4;
			correlations[2] = 0.1;
			Sample::CholeskyUpperTriangularMatrix<double> choleskyUpper( correlations, SHARES_COUNT );

			//reduced to float
			#pragma warning(disable : 4305)
			float expected_results [SHARES_COUNT][SHARES_COUNT] = {
				{ 1.00000000, 0.35000000, -0.40000000 },
				{ 0.00000000, 0.93674970, 0.25620505 },
				{ 0.00000000, 0.00000000, 0.87997669 }
			};

			for( int i  = 0; i < SHARES_COUNT; i++ ) {
				for( int j = 0; j < SHARES_COUNT; j++ ) {
					Assert::AreEqual( (float)choleskyUpper(i,j), expected_results[i][j]);
				}
			}
		}
	};
}