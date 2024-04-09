#pragma once
#include <vector>
#include <iostream>
#include <tuple>
#include <stdexcept>
#include <algorithm>
#include <iostream>

namespace math
{
    template<class T>
    class matrix
    {
    public:
        matrix(unsigned int nRows, unsigned int nCols) :
            m_nRows( nRows ),
            m_nCols( nCols ),
            m_oData( nRows*nCols, 0 )
        {
            if ( !nRows || !nCols )
            {
                std::cerr << "invalid matrix size" << std::endl;
            }
        }

        static matrix identity( unsigned int nSize )
        {
            matrix oResult( nSize, nSize );

            int nCount = 0;
            std::generate( oResult.m_oData.begin(), oResult.m_oData.end(),
                [&nCount, nSize]() { return !(nCount++%(nSize + 1)); } );

            return oResult;
        }

        inline T& operator()(unsigned int nRow, unsigned int nCol)
        {
            if ( nRow >= m_nRows || nCol >= m_nCols )
            {
                std::cerr << "position out of range" << std::endl;
            }

            return m_oData[nCol+m_nCols*nRow];
        }

        inline matrix operator*(matrix& other)
        {
            if ( m_nCols != other.m_nRows )
            {
                std::cerr << "matrix dimensions are not multiplicable" << std::endl;
            }

            matrix oResult( m_nRows, other.m_nCols );
            for ( unsigned int r = 0; r < m_nRows; ++r )
            {
                for ( unsigned int ocol = 0; ocol < other.m_nCols; ++ocol )
                {
                    for ( unsigned int c = 0; c < m_nCols; ++c )
                    {
                        oResult(r,ocol) += (*this)(r,c) * other(c,ocol);
                    }
                }
            }

            return oResult;
        }

        inline matrix operator+(matrix& other)
        {
            if (m_nCols != other.m_nCols || m_nRows != other.m_nRows)
                std::cerr << "matrix dimensions are not suitable " << std::endl;
            matrix oResult(m_nRows, m_nCols);
            for (uint32_t  r = 0; r < m_nRows; ++r) {
                for (uint32_t c = 0; c < m_nCols; ++c) {
                    oResult(r, c) = (*this)(r, c) + other(r, c);
                }
            }
            return oResult;
        }

        inline matrix transpose()
        {
            matrix oResult( m_nCols, m_nRows );
            for ( unsigned int r = 0; r < m_nRows; ++r )
            {
                for ( unsigned int c = 0; c < m_nCols; ++c )
                {
                    oResult(c,r) += (*this)(r,c);
                }
            }
            return oResult;
        }

        inline unsigned int rows()
        {
            return m_nRows;
        }

        inline unsigned int cols()
        {
            return m_nCols;
        }

        inline std::vector<T> data()
        {
            return m_oData;
        }

        void print()
        {
            for ( unsigned int r = 0; r < m_nRows; r++ )
            {
                for ( unsigned int c = 0; c < m_nCols; c++ )
                {
                    std::cout << (*this)(r,c) << "\t";
                }
                std::cout << std::endl;
            }
        }

    private:
        unsigned int m_nRows;
        unsigned int m_nCols;
        std::vector<T> m_oData;
    };
};
