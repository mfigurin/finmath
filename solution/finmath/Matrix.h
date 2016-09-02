
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

			Matrix() {
			}
			void init( int rows, int cols, int fill ) {
				if( rows < 1 || cols < 1 ) {
					throw std::exception( "Invalid boundaries" );
				}
				_matrix = std::vector<std::vector<_T>>( rows, std::vector<_T>(cols, fill));
			}


		public:

			Matrix( int rows, int cols, int fill = 0 ) {
				init( rows, cols, fill );
			}
			Matrix( const Matrix& matrix ) {
				_matrix = matrix._matrix;
			}

			int rows() const { return _matrix.size(); } 
			int cols() const { return _matrix[0].size(); } 

			_T inline operator() ( int row, int col ) const {
				return _matrix.at(row).at(col);
			}
			_T inline &operator() ( int row, int col ) {
				return _matrix.at(row).at(col);
			}

			Matrix<_T> operator* ( const Matrix<_T>& matrix ) {
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
	class CholeskyLowerTriangularMatrix : public Matrix<_T> {
	
		public:
			CholeskyLowerTriangularMatrix( std::vector<_T> correlation_values, int n = 0 )  {
				if( n == 0 ) {
					n = (1 + std::sqrt(1 + correlation_values.size()*8))/2;
				}
				init( n, n, 0 );
				Matrix a( n, n );
				for( int i = 0, v = 0; i < n; i++ ) {
					for( int j = 0; j <= i; j++ ) {
						a(i,j) = (i == j ? 1.0 : correlation_values.at(v++)); // fill up lower triangular of the matrix with diagonal
					}
				}
				compute_ll( *this, a );
			}
		
			//exaclty as at https://en.wikipedia.org/wiki/Cholesky_decomposition#The_Cholesky_algorithm
			static void compute_ll( Matrix<_T>& ll, const Matrix<_T>& a ) 
			{
				for( int i = 0; i < ll.rows(); i++)
					for( int j = 0; j <= i; j++) {
						double sum = 0.;
						for( int k = 0; k < j; k++ ) {
							sum += ll(i,k) * ll(j,k); // alg: i == j ?  ll(j,k) * ll(j,k) : ll(i,k) * ll(j,k);
						}
						_T v = a(i,j) - sum;
						if( i == j ) {
							if( v < 0. ) {
								throw std::exception( "The matrix is not positive" );
							}
							ll(j,j) = sqrt( v ); // alg: sqrt( a(j,j) - sum );
						}else{
							ll(i,j) = v / ll(j,j); // alg: (1./ll(j,j)) * (a(i,j) - sum);
						}
				 }
			}
	};

	template<typename _T = double> 
	class CholeskyUpperTriangularMatrix : public CholeskyLowerTriangularMatrix<_T> {
	
		public:
			CholeskyUpperTriangularMatrix( std::vector<_T> correlation_values, int n = 0 ) : CholeskyLowerTriangularMatrix<_T>( correlation_values, n ) {
				//transpose to upper triangular form
				for (int i = 0; i < rows(); i++) {
					for (int j = 0; j < i; j++)
						std::swap( (*this)(j,i), (*this)(i,j) );
				}
			}
	};


}

