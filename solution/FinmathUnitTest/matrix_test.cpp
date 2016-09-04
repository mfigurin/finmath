#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../finmath/matrix.h"

namespace FinmathUnitTest
{
	TEST_CLASS(Matrix2Test)
	{

	public:

		TEST_METHOD(MatrixTest1)
		{
			finmath::Matrix matrix1(1,3,1.0);
			finmath::Matrix matrix2(3,3,1.0);
			finmath::Matrix result = matrix1*matrix2;
			Assert::IsTrue( finmath::Matrix(1,3, 3.0) == result);
		}

		TEST_METHOD(VectorTest1)
		{
			finmath::Matrix vector(3,1, 0.5);
			finmath::Matrix matrix(3,3, 1);
			finmath::Matrix result = matrix * vector;
			Assert::IsTrue( finmath::Matrix(3,1, 1.5) == result);
		}

		TEST_METHOD(CorrelationMatrixTest1)
		{
			finmath::CorrelationMatrix corr(3);
			corr.set_correlation(0, 1, 0.5);
			Assert::AreEqual( 3, (int) corr.rows());
			Assert::AreEqual( 3, (int) corr.cols());
			Assert::AreEqual( 1.0, corr(1,1));
			Assert::AreEqual( 0.5, corr(0,1));
			Assert::AreEqual( 0.5, corr(1,0));
		}

		TEST_METHOD(CorrelationMatrixTest2)
		{
			finmath::CorrelationMatrix corr(3);
			corr.set_correlation( 0, 1, 0.35 );
			corr.set_correlation( 0, 2, -0.4 );
			corr.set_correlation( 1, 2, 0.1 );
			finmath::Matrix choleskyUpper = corr.cholesky_decomposition().transpose();

			//reduced to float
			#pragma warning(disable : 4305)
			double expected_results [3][3] = {
				{ 1.00000000, 0.35000000, -0.40000000 },
				{ 0.00000000, 0.93674970, 0.25620505 },
				{ 0.00000000, 0.00000000, 0.87997669 }
			};

			finmath::Matrix expectedMatrix( 3,3, (double*)&expected_results );
			Assert::IsTrue( expectedMatrix == choleskyUpper);
		}
	};
}