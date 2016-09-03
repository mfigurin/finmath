
#pragma once

#include <math.h>
#include <vector>

namespace finmath {

	class Matrix {

	protected:
		std::vector<std::vector<double>> matrix_;

	public:

		Matrix( int rows, int cols, double fill = 0 );
		Matrix( const Matrix& matrix );

		int inline rows() const { return matrix_.size(); }
		int inline cols() const { return matrix_[0].size(); }

		double inline operator() ( int row, int col ) const {
			return matrix_.at(row).at(col);
		}

		double inline &operator() ( int row, int col ) {
			return matrix_.at(row).at(col);
		}

		// vector index operators
		double inline operator() ( int index ) const {
			if (cols() == 1 )
				return (*this)(index,0);
			else if (rows() == 1)
				return (*this)(0,index);
			throw std::exception("The matrix is not a vector" );
		}

		double inline &operator() ( int index ) {
			if (cols() == 1 )
				return (*this)(index,0);
			else if (rows() == 1)
				return (*this)(0,index);
			throw std::exception("The matrix is not a vector" );
		}

		Matrix operator* ( const Matrix& matrix );
		Matrix transpose(void);
		Matrix cholesky_decomposition();			
	};

	class CorrelationMatrix : public Matrix {
	public:
		CorrelationMatrix( unsigned int size, double fill = 0);		
		void set_correlation(unsigned int row, unsigned int col, double value);
	};
}

