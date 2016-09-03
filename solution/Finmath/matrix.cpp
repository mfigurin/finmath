
#include "stdafx.h"
#include "matrix.h"

namespace finmath {

	Matrix::Matrix( int rows, int cols, double fill){
		if( rows < 1 || cols < 1 ) {
			throw std::exception( "Invalid matrix size" );
		}
		matrix_ = std::vector<std::vector<double>>( rows, std::vector<double>(cols, fill));
	}

	Matrix::Matrix( const Matrix& matrix ) {
		matrix_ = matrix.matrix_;
	}

	Matrix Matrix::operator* ( const Matrix& matrix ) {
		if( cols() != matrix.rows()) {
			throw std::exception( "Incompatible matrixes sizes for multiplication" );
		}
		Matrix mult( rows(), matrix.cols() );
		for( int i = 0; i < mult.rows(); i++ ) {
			for( int j  = 0; j < mult.cols(); j++ ) { 
				for( int k = 0; k < cols(); k++ ) {
					mult(i,j) += (*this)(i,k) * matrix(k,j);
				}
			}
		}
		return mult;
	}

	Matrix Matrix::transpose(void) 
	{
		Matrix transposed( cols(), rows() );
		for (int i = 0; i < rows(); i++) {
			for (int j = 0; j < cols(); j++)
				transposed(j,i) = (*this)(i,j);
		}
		return transposed;
	}

	Matrix Matrix::cholesky_decomposition() 
	{
		Matrix l(rows(),cols());

		for( int i = 0; i < rows(); i++){
			for( int j = 0; j <= i; j++) {
				double sum = 0.;
				for( int k = 0; k < j; k++ ) {
					sum += l(i,k) * l(j,k); // alg: i == j ?  l(j,k) * l(j,k) : l(i,k) * l(j,k);
				}
				double v = (*this)(i,j) - sum;
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

	CorrelationMatrix::CorrelationMatrix( unsigned int size, double fill) : Matrix(size,size,fill) {
		for (unsigned int i = 0; i < size; i++)
			(*this)(i,i) = 1.0;
	}

	void CorrelationMatrix::set(unsigned int row, unsigned int col, double value) {
		(*this)(row,col) = value;
		(*this)(col,row) = value;
	}
}

