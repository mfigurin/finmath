
#pragma once

#include <math.h>
#include <vector>
#include <exception>

namespace Sample {

	//use template only to have implementation code here and avoid accompanied cpp (sbj to change?)
	template<typename _T = double> 
	class Matrix {
	
		protected:
		std::vector<std::vector<_T>> _matrix;

		public:

			Matrix( int rows, int cols, int fill = 0 ) {
				if( rows < 1 || cols < 1 ) {
					throw std::exception( "Invalid boundaries" );
				}
				_matrix = std::vector<std::vector<_T>>( rows, std::vector<_T>(cols, fill));
			}
			Matrix( const Matrix& matrix ) {
				_matrix = matrix._matrix;
			}

			int inline rows() { return _matrix.size(); }
			int inline cols() { return _matrix[0].size(); }

			_T inline &operator() ( unsigned int row, unsigned int col ) {
				return _matrix.at(row).at(col);
			}
			_T inline operator() ( unsigned int row, unsigned int col ) const {
				return _matrix.at(row).at(col);
			}

			Matrix<_T> operator* ( Matrix<_T>& matrix ) {
				Matrix<_T> mult( rows(), matrix.cols() );
				for( int i = 0; i < mult.rows(); i++ ) {
					for( int j  = 0; j < mult.cols(); j++ ) { 
						for( int k = 0; k < cols(); k++ ) {
							mult(i,j) += (*this)(i,k) * matrix(k,j);
						}
					}
				}
				return mult;
			}
			
	};


	template<typename _T = double> 
	class CholeskyUpperTriangularMatrix : public Matrix<_T> {
	
		public:
			CholeskyUpperTriangularMatrix( std::vector<_T> correlation_values, int n ) : Matrix<_T>( n, n ) {
				Matrix a(n,n);
				for( int i = 0, v = 0; i < n; i++ ) {
					for( int j = 0; j <= i; j++ ) {
						a(i,j) = (i == j ? 1.0 : correlation_values.at(v++)); // fill up lower triangular of the matrix with diagonal
					}
				}
				compute_llt( *this, a );
			}
		
			//exaclty as at https://en.wikipedia.org/wiki/Cholesky_decomposition#The_Cholesky_algorithm
			static void compute_llt( Matrix<_T>& llt, const Matrix<_T>& a ) 
			{
				for( int i = 0; i < llt.rows(); i++)
					for( int j = 0; j <= i; j++) {
						double sum = 0.;
						for( int k = 0; k < j; k++ ) {
							sum += llt(i,k) * llt(j,k); // alg: i == j ?  llt(j,k) * llt(j,k) : llt(i,k) * llt(j,k);
						}
						_T v = a(i,j) - sum;
						if( i == j ) {
							if( v < 0. ) {
								throw std::exception( "The matrix is not positive" );
							}
							llt(j,j) = sqrt( v ); // alg: sqrt( a(j,j) - sum );
						}else{
							llt(i,j) = v / llt(j,j); // alg: (1./llt(j,j)) * (a(i,j) - sum);
						}
				 }
				//transpose to upper triangular form
				for (int i = 0; i < llt.rows(); i++) {
					for (int j = 0; j < i; j++)
						std::swap( llt(j,i), llt(i,j) );
				}
			}
	};

}

