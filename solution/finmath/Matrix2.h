
#pragma once

#include <math.h>
#include <vector>
#include <exception>

namespace Sample {

	//use template only to have implementation code here and avoid accompanied cpp (sbj to change?)
	template<typename _T = double> 
	class Matrix2 {
	
		protected:
		std::vector<std::vector<_T>> _matrix;

		public:

			Matrix2( unsigned int rows, unsigned int cols, double fill = 0 ){
				if( rows < 1 || cols < 1 ) {
					throw std::exception( "Invalid boundaries" );
				}
				_matrix = std::vector<std::vector<_T>>( rows, std::vector<_T>(cols, fill));
			}
			Matrix2( const Matrix2& matrix ) {
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

			Matrix2<_T> operator* ( Matrix2<_T>& matrix ) {
				Matrix2<_T> mult( rows(), matrix.cols() );
				for( int i = 0; i < mult.rows(); i++ ) {
					for( int j  = 0; j < mult.cols(); j++ ) { 
						for( int k = 0; k < cols(); k++ ) {
							mult(i,j) += (*this)(i,k) * matrix(k,j);
						}
					}
				}
				return mult;
			}

			Matrix2<_T> transpose(void) 
			{
				Matrix2<_T> transposed( cols, rows );
				for (int i = 0; i < rows(); i++) {
					for (int j = 0; j < cols(); j++)
						transposed(j,i) = (*this)(i,j);
				}
				return transposed;
			}

			Matrix2<_T> cholesky_decomposition() 
			{
				Matrix2<_T> l(rows(),cols());

				for( int i = 0; i < rows(); i++){
					for( int j = 0; j <= i; j++) {
						double sum = 0.;
						for( int k = 0; k < j; k++ ) {
							sum += l(i,k) * l(j,k); // alg: i == j ?  l(j,k) * l(j,k) : l(i,k) * l(j,k);
						}
						_T v = (*this->)(i,j) - sum;
						if( i == j ) {
							if( v < 0. ) {
								throw std::exception( "The matrix is not positive" );
							}
							l(j,j) = sqrt( v ); // alg: sqrt( this->(j,j) - sum );
						}else{
							l(i,j) = v / l(j,j); // alg: (1./l(j,j)) * (this->(i,j) - sum);
						}
					}
				}
				return l;
			}			
	};

	template<typename _T = double> 
	class Vector : public Matrix2<_T> {
	public:
		Vector( unsigned int size, double fill = 0) : Matrix2<_T>(1,size,fill) {
		}
		
		_T inline &operator() ( unsigned int index ) {
				return this->(0,index);
		}
			
		_T inline operator() ( unsigned int index ) const {
			return return this->(0,index);
		}

	};

	template<typename _T = double> 
	class CorrelationMatrix : public Matrix2<_T> {
	public:
		CorrelationMatrix( unsigned int size, double fill = 0) : Matrix2<_T>(size,size,fill) {
			for (int i = 0; i < size; i++)
				(*this)(i,i) = 1.0;
		}
		
		void set(unsigned int row, unsigned int col, double value) {
				(*this)(row,col) = value;
				(*this)(col,row) = value;
		}
	};
}

