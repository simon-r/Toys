#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <utility>
#include <cstdlib>
#include <ctime> 

// Copyright (C) 2011  Simone Riva
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

// g++ random_labyrinth.cpp -o random_labyrinth -std=c++14

using namespace std ;

template <class Type>
class Matrix {
public :
    
    Matrix() : _cols(0) , _rows(0) {} 
    
    Matrix( unsigned int cols , unsigned int rows , Type def = Type() ) : _cols(cols) , _rows(rows) {
        _matrix.resize( cols*rows , def ) ;
    }
    
    Matrix( unsigned int size , Type def = Type() ) : _cols(size) , _rows(size) {
        _matrix.resize( size*size , def ) ;
    }
    
    void resize( unsigned int size ,  Type def = Type() ) {
        _rows = size ;
        _cols = size ;
        _matrix.resize( size*size , def ) ;
    }
    
    unsigned int cols() { return _cols ; }
    unsigned int rows() { return _rows ; }
    
    void print_matrix() {
        cout << endl ;
        for ( int i = 0 ; i < _rows ; i++ ) {
            for ( int j = 0 ; j < _cols ; j++ ) {
                cout << _matrix[ _cols*j + i ] ;
            }
            cout << endl ;
        }
    }
    
    void set( unsigned int i , unsigned int j , const Type& el ) {
        _matrix[ _cols*j + i ] = el ;
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
    
    Type get( unsigned int i , unsigned int j ) {
        return _matrix[ _cols*j + i ] ;
    }
    
private :
    
    vector<Type> _matrix ;
    
    unsigned int _cols ;
    unsigned int _rows ;
    
} ;

class MazeSolutionNode {
    friend class MazeSolutionTree ;
public:
    MazeSolutionNode() : parent(nullptr) {}
    
    int get_i() { return _i ; }
    int get_j() { return _j ; }
    
    void set_i( int i ) { _i = i ; } 
    void set_j( int j ) { _j = j ; } 
    
    void set_ij( int i , int j ) {
        set_i( i ) ;
        set_j( j ) ;
    }
    
    void set_dir( int di , int dj ) { _di = di ; _dj = dj ; }
    
    int get_di() { return _di ; }
    int get_dj() { return _dj ; }
    
    bool operator= ( MazeSolutionNode other ) {
        if ( _i == other._i and _j == other._j and _di == other._di and _dj == other._dj ) 
            return true ;
        else
            return false ;
    }
    
    MazeSolutionNode* get_parent() {
        return parent ;
    }
    
    void add_leaf( MazeSolutionNode *nd ) {
        nd->parent = this ;
        leaves.push_back( nd ) ;
    }
    
    void remove_leaf( MazeSolutionNode *nd ) {
        leaves.remove( nd ) ;
    }
    
    void add_path_coord( int i , int j ) {
        path.push_back( pair<int,int>( i , j ) ) ;
    }
    
    int leaves_cnt() {
        return leaves.size() ;
    }
    
    list< pair<int,int> >* get_path() {
        return &path ;
    }
    
    list<MazeSolutionNode*>* get_leaves() {
        return &leaves ;
    }
    
private:
    int _i ;
    int _j ;
    
    int _di ;
    int _dj ;
    
    list<MazeSolutionNode*> leaves ;
    MazeSolutionNode* parent ;
    
    list< pair<int,int> > path ;
} ;

class MazeSolutionTree {
public :
    
    MazeSolutionTree() : head(nullptr) {}
    
    bool set_head( MazeSolutionNode* he ) {
        if ( head != nullptr ) {
            return false ;
        }
        
        head = he ;
        
        return true ;
    }
    
    void set_solution_node( MazeSolutionNode* nd ) {
        solution_node = nd ;
    }
    
    void purge_tree_rec( MazeSolutionNode* nd ) {
        MazeSolutionNode* parent = nd->get_parent() ;
        if ( parent == nullptr ) 
            return ;
        
        for ( auto itr = parent->leaves.begin() ; itr != parent->leaves.end() ; ) {
            if ( *itr != nd )
                itr = parent->leaves.erase( itr ) ;
            else
                itr++ ;
        }
        
        purge_tree_rec( parent ) ;
    }
    
    void purge_tree() {
        purge_tree_rec( solution_node ) ;
    }
    
    MazeSolutionNode* get_head() {
        return head ;
    }
    
    MazeSolutionNode* remove_leaf( MazeSolutionNode* nd ) {
        
        if( nd->get_parent() == nullptr ) 
            return nd ;
        
        MazeSolutionNode* pr = nd->get_parent() ;
        pr->remove_leaf( nd ) ;
        
        delete nd ;
        nd = 0 ;
        
        if ( pr->leaves.size() == 0 ) {
            pr = remove_leaf( pr ) ;
        }
        
        return pr ;
    }
    
private :
    MazeSolutionNode* head ;
    MazeSolutionNode* solution_node ;
} ;


class Maze {
public :
    Maze( int size ) : _size(size) {
        _maze.resize( size , false ) ;
        _sol.resize( size , 0 ) ;
    }
    
    bool allowed( int i , int j ) {
        if ( _maze.get( i+1 , j ) + _maze.get( i-1 , j ) + _maze.get( i , j+1 ) + _maze.get( i , j-1 ) == 1 )
            return true ;
        else 
            return false ;
    }
    
    bool allowed_coord( int i , int j ) {
        
        if ( i <= 0 or j <= 0 or i >= this->size() - 1 or j >= this->size() - 1 )
            return false ;
        else if ( _maze.get( i , j ) )
            return false ;
        else
            return true ;
    }
    
    bool allowed_maze_coord( int i , int j ) {
        
        if ( i < 0 or j < 0 or i > this->size() - 1 or j > this->size() - 1 )
            return false ;
        else if ( not _maze.get( i , j ) )
            return false ;
        else
            return true ;
    }
    
    int start_coords_j() {
        int j = rand() % this->size() ;
        
        _si = this->size() - 1 ;
        _sj = j ;
        
        _maze.set( this->size() - 1 , j , true ) ;    
        _maze.set( this->size() - 2 , j , true ) ;
        
        pair<int,int> p( this->size() - 2 , j ) ;
        
        _maze_coord.push_front( p ) ;
        
        return j ;
    }
    
    bool maze_agent( int i , int j ) {
        
        vector<pair<int,int> > coord_vect(4) ;
        
        while( true ) {
            
            int vi = 0 ;
            
            for( int n = -1 ; n <= 1 ; n+=2 ) {
                int ni = i + n ;
                int nj = j + n ;
                
                if ( allowed_coord( ni , j ) and allowed( ni , j ) ) {
                    coord_vect[vi] =  pair<int,int>( ni , j ) ;
                    vi++ ;
                }
                
                if( allowed_coord( i , nj ) and allowed( i , nj ) ) {
                    coord_vect[vi] =  pair<int,int>( i , nj ) ;
                    vi++ ;
                }
            }
            
            if ( vi == 0 )
                return false ;
            
            int r = rand() % vi ;
            
            i = coord_vect[r].first ;
            j = coord_vect[r].second ;
            
            _maze.set( i , j , true ) ;
            
            pair<int,int> p( i , j ) ;
            _maze_coord.push_front( p ) ;
        }
    }
    
    void add_exit() {
        vector<int> exit( this->size() ) ;
        int cnt = 0 ;
        for( int j = 0 ; j < this->size() ; j++ ) {
            if ( _maze.get(1,j) ) {
                exit[cnt] = j ;
                cnt++ ;
            }
        }
        
        int r = rand() % cnt ;
        _maze.set( 0 , exit[r] , true ) ;
    }
    
    pair<int,int> get_rand_maze_point() {
        int r = rand() % _maze_coord.size() ;
        int cnt = 0 ;
        auto itr = _maze_coord.begin() ;
        for(; itr != _maze_coord.end() ; itr++ ) {
            if ( cnt == r ) break ;
            cnt++ ;
        }
        
        pair<int,int> res = *itr ;
        _maze_coord.erase( itr ) ;
        
        return res ;
    }
    
    void build_maze() {
        int j = this->start_coords_j() ;
        int i = this->size() - 2 ;
        
        maze_agent( i , j ) ;
        
        while( _maze_coord.size() > 0 ) {
            
            pair<int,int> c = get_rand_maze_point() ;
            maze_agent( c.first , c.second ) ;
        }
        
        add_exit() ; 
    }
    
    // Solver ////////////////////////////////////////////////////////////
        
    bool solver_agent( MazeSolutionNode* parent , stack<MazeSolutionNode*> &forks ) {
        vector<pair<int,int> > coord_vect(4) ;
        
        int i = parent->get_i() ;
        int j = parent->get_j() ;
        
        bool first_step = true ;
        
        while( true ) {
            int vi = 0 ;
            
            if( i == 0 )
                cout << i << "   " << j << endl ;
            
            if( i == 0 ) {
                sol_tree.set_solution_node( parent ) ;
                return true ;
            }
            
            for( int n = -1 ; n <= 1 ; n+=2 ) {
                int ni = i + n ;
                int nj = j + n ;
                
                if ( allowed_maze_coord( ni , j ) and _maze.get( ni , j ) and _sol.get( ni, j ) != -1 ) {
                    coord_vect[vi] =  pair<int,int>( ni , j ) ;
                    vi++ ;
                }
                
                if ( allowed_maze_coord( i , nj ) and _maze.get( i , nj ) and _sol.get( i, nj ) != -1 ) {
                    coord_vect[vi] =  pair<int,int>( i , nj ) ;
                    vi++ ;
                }
            }
            
            if ( vi == 0 ) {
                
                break ;
            
            } else if ( vi >= 2 ) {
                 
                for ( int n = 0 ; n < vi ; n++ ) {
                    
                    MazeSolutionNode* leaf = new MazeSolutionNode() ;
                    
                    i = coord_vect[n].first ;
                    j = coord_vect[n].second ;
                    
                    leaf->set_i( i ) ;
                    leaf->set_j( j ) ;
                    
                    leaf->add_path_coord( i , j ) ;
                    
                    leaf->set_dir( coord_vect[vi].first , coord_vect[vi].second ) ;
                    
                    parent->add_leaf( leaf ) ;
                    forks.push( leaf ) ;
                }
                
                parent->add_path_coord( i , j ) ;
                
                _sol.set( i , j , -1 ) ;
                
                break ;
            } else {
                
                i = coord_vect[0].first ;
                j = coord_vect[0].second ;
            
                _sol.set( i , j , -1 ) ;
                
                parent->add_path_coord( i , j ) ;   
            }
        }
        
        return false ;
    }
    

    
    void solve_maze() {
        stack<MazeSolutionNode*> forks ;
        
        MazeSolutionNode* head = new MazeSolutionNode() ;
        sol_tree.set_head( head ) ;
        
        head->set_ij( _si , _sj ) ;
        head->add_path_coord( _si , _sj ) ;
        
        head->set_dir( -1 , 0 ) ;
        
        forks.push( head ) ;
        
        while( forks.size() > 0 ) {
            MazeSolutionNode* parent = forks.top() ;
            forks.pop() ;
            
            bool fs = solver_agent( parent , forks ) ;
            
            if( fs )
                break ;
            
            //cout << parent->leaves_cnt() << endl ;
            
            if( parent->leaves_cnt() == 0 ) {
                sol_tree.remove_leaf( parent ) ;
            }
            
        }

        sol_tree.purge_tree() ;
        set_solution() ;
        
    }
    
    // utils ////////////////////////////////////////////////////////////////
    
    void set_solution() {
        
        stack<MazeSolutionNode*> forks ;
        
        MazeSolutionNode* head = sol_tree.get_head() ;
        
        forks.push( head ) ;
        
        while( forks.size() > 0 ) {
            list<pair<int,int> >* path ;
            MazeSolutionNode* parent ;
            
            parent = forks.top() ;
            forks.pop() ;
            
            path = parent->get_path() ;
            
            for( auto itr = path->begin() ; itr != path->end() ; ++itr ) {
                if( itr == path->begin() ) 
                    _sol.set( itr->first , itr->second , 2 ) ;
                else 
                    _sol.set( itr->first , itr->second , 1 ) ;
            }
            
            list<MazeSolutionNode*>* leaves = parent->get_leaves() ;
            for( auto itr = leaves->begin() ; itr != leaves->end() ; ++itr ) {
                forks.push( *itr ) ;
            }
        }
    }
    
    void print_maze() {
        cout << endl ;
        for ( int i = 0 ; i < this->size() ; i++ ) {
            for ( int j = 0 ; j < this->size() ; j++ ) {
                if ( _maze.get(i,j) and _sol.get(i,j) < 1 ) 
                    cout << char(27)+string("[47m0")+char(27)+string("[49m") ;
                
                else if ( ( _maze.get(i,j) and _sol.get(i,j) == 1 ) )
                    cout << char(27)+string("[42m1")+char(27)+string("[49m") ;
                
                else if ( ( _maze.get(i,j) and _sol.get(i,j) == 2 ) )
                    cout << char(27)+string("[42m2")+char(27)+string("[49m") ;
                
                else if ( ( not _maze.get(i,j) and _sol.get(i,j) >= 1 ) )
                    cout << char(27)+string("[41mR")+char(27)+string("[49m") ;
                
                else
                    cout << " " ;
            }
            cout << endl ;
        }
    }
    
    int size() { return _size ; }
    
private :
    
    int _size ;
    Matrix<bool> _maze ;
    Matrix<int> _sol ;
    
    int _si , _sj ;
    
    list< pair<int,int> > _maze_coord ;
    
    MazeSolutionTree sol_tree ;
} ;


int main ( void ) {
    
    Maze m(40) ;
    
    srand(time(NULL)) ;
    
    m.build_maze() ;
    
    m.solve_maze() ;
    
    m.print_maze() ;
    
    
    return 0 ;
}