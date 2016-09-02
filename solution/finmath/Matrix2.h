
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

			Matrix2( int rows, int cols, _T fill = 0 ){
				if( rows < 1 || cols < 1 ) {
					throw std::exception( "Invalid boundaries" );
				}
				_matrix = std::vector<std::vector<_T>>( rows, std::vector<_T>(cols, fill));
			}
			Matrix2( const Matrix2& matrix ) {
				_matrix = matrix._matrix;
			}

			int inline rows() const { return _matrix.size(); }
			int inline cols() const { return _matrix[0].size(); }
			
			_T inline operator() ( int row, int col ) const {
				//if( row < 0 || col < 0 || row >= rows() || col >= cols()) {
				//	throw std::exception( "Invalid item index" );
				//}
				return _matrix.at(row).at(col);
			}
			_T inline &operator() ( int row, int col ) {
				return _matrix.at(row).at(col);
			}

			Matrix2<_T> operator* ( const Matrix2<_T>& matrix ) {
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
				Matrix2<_T> transposed( cols(), rows() );
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
						_T sum = 0.;
						for( int k = 0; k < j; k++ ) {
							sum += l(i,k) * l(j,k); // alg: i == j ?  l(j,k) * l(j,k) : l(i,k) * l(j,k);
						}
						_T v = (*this)(i,j) - sum;
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

			// Vector index operators
			_T inline &operator() ( int index ) {
				if (cols() == 1 )
					return (*this)(index,0);
				else if (rows() == 1)
					return (*this)(0,index);
				else
					throw std::exception( "The matrix is not a vector" );
			}
			
			_T inline operator() ( int index ) const {
				if (cols() ==1 )
					return this->(index,0);
				else if (rows() == 1)
					return this->(0,index);
				else
					throw std::exception( "The matrix is not a vector" );
			}
	};

	//// row matrix; when multiplied by matrix simulates column matix
	//template<typename _T = double> 
	//class Vector : public Matrix2<_T> {
	//public:
	//	Vector( int size, double fill = 0) : Matrix2<_T>(1,size,fill) {
	//	}
	//	
	//	_T inline operator() ( int index ) const {
	//		return (*this)(0,index);
	//	}
	//	_T inline &operator() ( int index ) {
	//		return (*this)(0,index);
	//	}
	//	_T inline operator() ( int row, int col ) const {
	//		if( row > 0 && col > 0 ) {
	//			throw std::exception( "Invalid: both indexes above zero." );
	//		}
	//		return Matrix2<_T>( 0, row + col ) ;
	//	}
	//	_T inline &operator() ( int row, int col ) {
	//		if( row > 0 && col > 0 ) {
	//			throw std::exception( "Invalid: both indexes above zero." );
	//		}
	//		return Matrix2<_T>( 0, row + col ) ;
	//	}

	//	Vector<_T> operator* ( const Matrix2<_T>& matrix ) {
	//		return (Vector<_T>)Matrix2<_T>::operator*(matrix);
	//	}
	//	Vector<_T> operator* ( const Vector<_T>& vector ) {
	//		return (Vector<_T>)Matrix2<_T>::operator*(matrix);
	//	}

	//};

	template<typename _T = double> 
	class CorrelationMatrix : public Matrix2<_T> {
	public:
		CorrelationMatrix( unsigned int size, _T fill = 0) : Matrix2<_T>(size,size,fill) {
			for (unsigned int i = 0; i < size; i++)
				(*this)(i,i) = 1.0;
		}
		
		void set(unsigned int row, unsigned int col, double value) {
				(*this)(row,col) = value;
				(*this)(col,row) = value;
		}
	};
}

