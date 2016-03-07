#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <utility>
#include <cstdlib>
#include <ctime> 

// Copyright (C) 2015  Simone Riva
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// g++ xxx.cpp -o xxx -std=c++14

 

#ifndef _MATRIX_H
#define _MATRIX_H

using namespace std ;

template <class Type>
class Matrix {
public :
    
    Matrix() : _cols(0) , _rows(0) , _t(false) {} 
    
    Matrix( unsigned int cols , unsigned int rows , Type def = Type() ) : _cols(cols) , _rows(rows) , _t(false) {
        _matrix.resize( cols*rows , def ) ;
    }
    
    Matrix( unsigned int size , Type def = Type() ) : _cols(size) , _rows(size) , _t(false) {
        _matrix.resize( size*size , def ) ;
    }
    
    void resize( unsigned int size ,  Type def ) {
        _rows = size ;
        _cols = size ;
        _matrix.resize( size*size  , def ) ;
    }
    
    void resize( unsigned int rows , unsigned int cols ,  Type def ) {
        _rows = rows ;
        _cols = cols ;
        _matrix.resize( rows*cols , def ) ;
    }
    
    unsigned int cols() const { return _cols ; }
    unsigned int rows() const { return _rows ; }
    
    const void print_matrix() {
        for ( int i = 0 ; i < _rows ; i++ ) {
            for ( int j = 0 ; j < _cols ; j++ ) {
                cout << _matrix[ p_indx(i,j) ] << " " ;
            }
            cout << endl ;
        }
    }
    
    void set( unsigned int i , unsigned int j , const Type& el ) {
        _matrix[ p_indx(i,j) ] = el ;
    }
    
    Type get( unsigned int i , unsigned int j ) const {
        return _matrix[ p_indx(i,j) ] ;
    }
    
    Type& operator() ( unsigned int i , unsigned int j ) {
        return _matrix[ p_indx(i,j) ] ;
    }
    
    void T() {
        _t = not _t ;
        
        unsigned int tmp ;
        tmp = _rows ;
        _rows = _cols ;
        _cols = tmp ;
    }
    
    bool isT() {
        return _t ;
    }
    
    void set_iterator( auto begin , auto end ) {
        
        unsigned int i = 0 , j = 0 ;
        
        for ( auto itr = begin ; itr != end ; itr++ ) {
            
            this->set( i , j , *itr ) ;
            cout << i << " " << j << " " << *itr << endl ;
            
            j++ ;
            if ( j >= _cols ) {
                j = 0 ;
                i++ ;
            }
        }
    }
    
private :
    
    inline unsigned int p_indx( unsigned int i , unsigned int j ) const {
        if ( _t )
            return (_cols*i + j) ;
        else 
            return (_rows*j + i) ;
    }
    
    vector<Type> _matrix ;
    
    unsigned int _cols ;
    unsigned int _rows ;
    
    bool _t ;  
} ; 


template <class Type>
class MatrixIterator {
public:
    
    MatrixIterator( const Matrix<Type>& mat , bool end ) : _i(0) , _j(0) {
        _matrix = &mat ;
        
        if ( end ) {
            _i = _matrix->rows() ;
        }
    }
    
    bool operator!= ( const MatrixIterator<Type>& other ) {
        
        if ( _i >= _matrix->rows() ) {
            return false ; // end point -> return false for stopping the iterator
        }
        
        if ( _j == other._j and _i == other._i )
            return false ;
        else
            return true ;
    }
    
    MatrixIterator<Type>& operator++ () {
        if ( _j < _matrix->cols() - 1 ) {
            _j++ ;  
        } else if ( _j >= _matrix->cols() - 1 ) {
            _j = 0 ;
            _i++ ;
        }
    }
    
    Type operator*() {
        return _matrix->get( _i , _j ) ;
    }
    
private:
    
    const Matrix<Type>* _matrix ;
    
    unsigned int _i ;
    unsigned int _j ;
} ;


template <class Type>
MatrixIterator<Type> begin( Matrix<Type>& mat ) {
    
    return MatrixIterator<Type>( mat , false ) ;
}

template <class Type>
MatrixIterator<Type> end( Matrix<Type>& mat ) {
    
    return MatrixIterator<Type>( mat , true ) ;
}

#endif
