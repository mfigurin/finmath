#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../finmath/matrix.h"

namespace FinmathUnitTest
{
	TEST_CLASS(Matrix2Test)
	{

	public:

		class Matrix4Test : public finmath::Matrix  {

			public:
			Matrix4Test( int rows, int cols, float fill) : Matrix( rows, cols, (double)fill ) {
			}
			Matrix4Test( int rows, int cols, float* fill) : Matrix( rows, cols ) {
				for( int i = 0; i < rows; i++ ) {
					for( int j  = 0; j < cols; j++ ) { 
						(*this)(i,j) = (double)*fill++;
					}
				}
			}

			bool operator== ( const Matrix& matrix ) const {
				if( cols() != matrix.cols() || rows() != matrix.rows() ) {
					return false; 
				}
				//reduced to float
				#pragma warning(disable : 4305)
				for( int i = 0; i < rows(); i++ ) {
					for( int j  = 0; j < cols(); j++ ) { 
						if( (float)(*this)(i,j) != (float)matrix(i,j)) {
							return false;
						}
					}
				}
				return true;
			}
		};



		TEST_METHOD(MatrixTest1)
		{
			finmath::Matrix matrix1(1,3,1.0);
			finmath::Matrix matrix2(3,3,1.0);
			finmath::Matrix result = matrix1*matrix2;
			Assert::AreEqual( Matrix4Test(1,3, 3.0) == result, true );
		}

		TEST_METHOD(VectorTest1)
		{
			finmath::Matrix vector(3,1, 0.5);
			finmath::Matrix matrix(3,3, 1);
			finmath::Matrix result = matrix * vector;
			Assert::AreEqual( Matrix4Test(3,1, 1.5) == result, true );
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
			float expected_results [3][3] = {
				{ 1.00000000, 0.35000000, -0.40000000 },
				{ 0.00000000, 0.93674970, 0.25620505 },
				{ 0.00000000, 0.00000000, 0.87997669 }
			};

			Matrix4Test expectedMatrix( 3,3, (float*)&expected_results );
			Assert::IsTrue( expectedMatrix == choleskyUpper);
		}

	};
}