#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../finmath/Matrix2.h"

namespace FinmathUnitTest
{
	TEST_CLASS(Matrix2Test)
	{
	public:

		TEST_METHOD(MatrixTest1)
		{
			Sample::Matrix2<double> matrix1(1,3,1.0);
			Sample::Matrix2<double> matrix2(3,3,1.0);
			Sample::Matrix2<double> result = matrix1*matrix2;

			Assert::AreEqual( (int) result.rows(), 1);
			Assert::AreEqual( (int) result.cols(), 3);

			for( int i  = 0; i < result.rows(); i++ ) {
				for( int j = 0; j < result.cols(); j++ ) {
					Assert::AreEqual( result(i,j), 3.0);
				}
			}
		}

		TEST_METHOD(VectorTest1)
		{
			Sample::Matrix2<double> vector(3,1,0.5);
			Sample::Matrix2<double> matrix(3,3,1);
			Sample::Matrix2<double> result = matrix * vector;

			Assert::AreEqual( (int) result.rows(), 3);
			Assert::AreEqual( (int) result.cols(), 1);

			for( int i  = 0; i < result.rows(); i++ ) {
				Assert::AreEqual( result(i), 1.5);
			}
		}

		TEST_METHOD(CorrelationMatrixTest1)
		{
			Sample::CorrelationMatrix<double> corr(3);
			corr.set(0,1, 0.5);
			Assert::AreEqual( 3, (int) corr.rows());
			Assert::AreEqual( 3, (int) corr.cols());
			Assert::AreEqual( 1.0, corr(1,1));
			Assert::AreEqual( 0.5, corr(0,1));
			Assert::AreEqual( 0.5, corr(1,0));
		}
	};
}