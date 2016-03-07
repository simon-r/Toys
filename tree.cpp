#include<iostream> 
#include<vector>
#include<list>
#include<map>
#include<stack>
#include<queue>
#include<tuple>
#include<cstdlib>
#include <time.h>


// g++ tree.cpp -o tree -std=c++14





using namespace std ;

class Node {
    
public:
    
    Node() {
        p_left = 0 ;
        p_right = 0 ;
        p_parent = 0 ;
        
        p_flag = false ;
    }
    
    Node( int key ) : Node() {
        p_value = key ;
    }
    
    Node* left() { return p_left ; }
    Node* right() { return p_right ; }
    
    int value() { return p_value ; }
    
    int key() { return value() ; }
    
    Node* set_left( int key ) {
        p_left = new Node( key ) ;
        p_left->set_parent( this ) ;
        return this->p_left ;
    }
    
    Node* set_left( Node* nd ) {
        if ( nd != 0 ) {
            p_left = nd ;
            p_left->set_parent( this ) ;
        } else {
            p_left = 0 ;
        }
        
        return this->p_left ;
    }
    
    Node* set_right( int value ) {
        this->p_right = new Node( value ) ;
        p_right->set_parent( this ) ;
        return this->p_right ;
    }
    
    Node* set_right( Node* nd ) {
        if ( nd != 0 ) {
            p_right = nd ;
            p_right->set_parent( this ) ;
        } else {
            p_right = 0 ;
        }
        
        return this->p_right ;
    }
    
    void set_parent( Node* parent ) {
        p_parent = parent ;
    }
    
    Node* parent() {
        return p_parent ;
    }
    
    int set_rand_priority() {
        p_rand_pri = rand() % 1000000 ;
        return p_rand_pri ;
    }
    
    int get_rand_priority() {
        return p_rand_pri ; 
    }
    
    bool is_terminal() {
        if ( left() == 0 and right() == 0 ) 
            return true ;
        else 
            return false ;
    }
    
    bool is_leaf() {
        return is_terminal() ;
    }
    
    bool flag() { 
        return p_flag ; 
    }
    
    void set_flag( bool flag ) {
        p_flag = flag ;
    }
    
    void set_element( int el ) {
        p_element = el ;
    }
    
    
private:
    
    int p_value ;
    
    int p_element ;

    bool p_flag ;
    
    int p_rand_pri ;

    Node *p_left ;
    Node *p_right ;
    
    Node *p_parent ;
} ;

class Tree ;

class tree_iterator {
    
public :
    
    tree_iterator( const Tree* t , bool f ) ;
    
    ~tree_iterator() {}
    
    bool operator!=(const tree_iterator& other) {
        
        if ( p_stn.size() == 0 and other.p_end or other.p_stn.size() == 0 and p_end ) {
            return false ;   
        } else if ( p_stn.size() == 0 and other.p_stn.size() == 0 ) {
            return false ;
        } else if ( p_end and other.p_end ) {
            return false ;
        } else if ( p_stn.size() > 0 and other.p_end or other.p_stn.size() > 0 and p_end ) {
            return true ;
        }
        
        Node* a = p_stn.top() ;
        Node* b = other.p_stn.top() ;
        
        if ( a != b )
            return true ;
        else 
            return false ;
    }
    
    tree_iterator& operator++() {
        if ( p_stn.size() == 0 ) {
            p_end = true ;
            return *this ;
        }
        
        Node* nd = p_stn.top() ;
        p_stn.pop() ;
        
        if ( nd->right() != 0 ) {
            Node* rn = nd->right() ;
            while( rn != 0 ) {
                p_stn.push( rn ) ;
                rn = rn->left() ;
            }
        }
    }
    
    int operator*() const {
        Node* nd = p_stn.top() ;
        return nd->value() ;
    }
    
private:
    
    bool p_end ;
    stack<Node*> p_stn ;   
};


class Tree {
    friend class tree_iterator ;
public:
    Tree() {
        p_tree = 0 ;
    }
    
    void add_value( int value ) {
        if ( p_tree == 0 ) {
            p_tree = new Node( value ) ;
            return ;
        }
        
        add_value_rec( this , p_tree , value ) ;
    }
    
    tree_iterator begin() {
        tree_iterator itr( this , true ) ;
        return itr ;
    }
    
    tree_iterator end() {
        tree_iterator itr( this , false ) ;
        return itr ;
    }
    
    // http://www.geeksforgeeks.org/treap-a-randomized-binary-search-tree/
    Node* insert_treap( int value ) {
        if ( p_tree == 0 ) {
            p_tree = new Node( value ) ;
            p_tree->set_rand_priority() ;
            return p_tree ;
        }
        
        Node* nd = add_value_rec( this , p_tree , value ) ;
        nd->set_rand_priority() ; 
        
        Node* parent = nd->parent() ; 
        Node* lnd = nd ;
        
        while( true ) {
        
            if ( lnd->get_rand_priority() > parent->get_rand_priority() ) {
                if ( parent->left() == lnd ) {
                    parent = right_rotation( parent ) ;
                } else {
                    parent = left_rotation( parent ) ;
                }
            } else 
                break ;
            
            lnd = parent ;
            parent = parent->parent() ; 
            
            if ( parent == 0 ) 
                break ;
        }
        
        return nd ;
    }
    
    // http://www.geeksforgeeks.org/treap-a-randomized-binary-search-tree/
    bool remove_treap( int value ) {
        
        if ( p_tree == 0 )
            return false ;
        
        Node* nd = search_rec( this , p_tree , value ) ;
        
        if ( nd == 0 )
            return false ;
                
        while( not nd->is_leaf() ) {
            if ( nd->left() == 0 and nd->right() != 0 ) 
                left_rotation( nd ) ;
            else if ( nd->left() != 0 and nd->right() == 0 ) 
                right_rotation( nd ) ;
            else if ( nd->left()->get_rand_priority() > nd->right()->get_rand_priority() ) 
                right_rotation( nd ) ;
            else
                left_rotation( nd ) ;        
        }
        
        Node* parent ;
        
        if ( nd->is_leaf() ) {
            
            parent = nd->parent() ;
            
            if ( parent == 0 ) {
                delete nd ;
                p_tree = 0 ;  
            } else {
                if ( parent->left() == nd ) 
                    parent->set_left( (Node*) 0  ) ;
                else 
                    parent->set_right( (Node*) 0  ) ;
                delete nd ;
            }
        }
        
        return true ;
    }
    
    Node* head() {
        return p_tree ;
    }
    
    void print_tree() {
        
        if ( p_tree == 0 )
            cout << "empty ... " ;
        else 
            print_tree_rec( this , p_tree ) ;
        
        cout << endl ;
    }
    
    void print_tree_struct() {
        if ( p_tree == 0 )
            cout << "empty ... " ;
        else 
            print_tree_struct_rec( this , p_tree ) ;
        
        cout << endl ;
    }
    
    void print_tree_struct_df() {
        
        stack<Node*> stl , str ;
        
        stl.push( p_tree ) ;
        
        while( not stl.empty() or not str.empty() ) {
        
            Node* node = stl.top() ;
            
            if ( node != 0 ) 
                cout << node->value() ;
            else 
                cout << "*" ;
            
            if( node->is_terminal() ) {
                //st.pop() ;
                continue ;
            }
            
            cout << "(" ;
            
            if( node->left() != 0 ) {
               // st.push( node->left() ) ;
            }
                
        }
    }
    
    Node* right_rotation( Node *Q ) {
        
        Node* parent = Q->parent() ;
        Node* P = Q->left() ;
        
        if ( P == 0 ) 
            return 0 ;
        
        Node* beta ;
        beta = P->right() ;
        
        P->set_right( Q ) ;
        
        Q->set_left( beta ) ;
        
        if( parent == 0 ) {
            p_tree = P ;
            P->set_parent( 0 ) ;
        } else {
            P->set_parent( parent ) ;
            
            if ( parent->left() == Q ) 
                parent->set_left( P ) ;
            else
                parent->set_right( P ) ;
            
        }
        
        return P ;
    }
    
    Node* left_rotation( Node *P ) {
        
        Node* parent = P->parent() ;
        Node* Q = P->right() ;
        
        if( Q == 0 ) 
            return 0 ;
        
        Node* beta ;
        
        beta = Q->left() ;
        
        P->set_right( beta ) ;
        Q->set_left( P ) ;
        
        if( parent == 0 ) {
            p_tree = Q ;
            Q->set_parent( 0 ) ;
        } else {
            
            Q->set_parent( parent ) ;
            
            if ( parent->left() == P ) 
                parent->set_left( Q ) ;
            else
                parent->set_right( Q ) ;
        }
        
        return Q ;
    }
    
    bool search ( int value ) {
        if( p_tree == 0 ) 
            return false ;
        else {
            Node* n = search_rec( this , p_tree , value ) ;   
            if ( n == 0 )
                return false ;
            else 
                return true ;
        }
    }
    
    int search_df( int value ) {
        int cnt = 0 ;
        
        stack<Node*> st ;
        
        if ( p_tree == 0 ) 
            return cnt ;
        
        st.push( p_tree ) ;
        
        while( st.size() > 0 ) {
            Node* node = st.top() ;
            st.pop() ;
            
            if( node->value() == value ) 
                cnt++ ;
            
            if ( node->left() != 0 )
                st.push( node->left() ) ;
            
            if ( node->right() != 0 )
                st.push( node->right() ) ;
        }
        
        return cnt ;
    }
    
    int search_bf( int value ) {
        int cnt = 0 ;
        
        if( p_tree == 0 )
            return cnt ;
        
        queue<Node*> q ;
        
        q.push( p_tree ) ;
        
        while( not q.empty() ) {
            Node *node ;
            
            node = q.front() ;
            q.pop() ;
            
            if ( node->value() == value ) 
                cnt++ ;
            
            if( node->left() != 0 )
                q.push( node->left() ) ;
            
            if( node->right() != 0 )
                q.push( node->right() ) ;
        }
        
        return cnt ;
    }
    
    int lower_depth() {
        
        queue<Node*> q ;
        queue<int> d ;
        
        q.push( p_tree ) ;
        d.push( 0 ) ;
        
        while( not q.empty() and not d.empty() ) {
            Node* nd ;
            int dp ;
            
            nd = q.front() ;
            dp = d.front() ;
            
            q.pop() ; 
            d.pop() ;
            
            if ( nd->is_terminal() )
                return dp ;
            
            if ( nd->left() != 0 ) {
                q.push( nd->left() ) ;
                d.push( dp+1 ) ;
            }
            
            if ( nd->right() != 0 ) {
                q.push( nd->right() ) ;
                d.push( dp+1 ) ;
            }            
        }
    }
    
    // http://www.geeksforgeeks.org/construct-a-binary-tree-from-parent-array-representation/
    void parent_array( const vector<int> &pa ) {
        
        multimap< int ,  int > nd_map ;
        Node* nd = 0 ;
        
        int cv ;
        
        for ( int i = 0 ; i < pa.size() ; i++ ) {
            
            if ( pa[i] == -1 ) {
                nd = new Node( i ) ;
                p_tree = nd ;
                cv = i ;
            } 
            
            nd_map.insert( pair<int,int>( pa[i] , i ) ) ;
        }
        
        queue<Node*> vlq ;
        
        while( true ) {
            
            if ( nd != 0 ) {
                int cnt = -1 ;
                Node* nnl = 0 , *nnr = 0 ;
                
                for ( auto it=nd_map.equal_range( nd->value() ).first ; 
                      it!=nd_map.equal_range( nd->value() ).second ; 
                      ++it ) {
                    
                    cnt ++ ;
                    if ( cnt == 0 ) {
                        int val = (*it).second ;
                        nd->set_left( val ) ;
                        nnl = nd->left() ;
                    } else {
                        int val = (*it).second ;
                        nd->set_right( val ) ;
                        nnr = nd->right() ;
                    }
                }
                    
                if ( cnt == -1 ) {
                    nd = 0 ;
                } else if ( cnt == 0 ) {
                    nd = nnl ;
                } else if ( cnt == 1 ) {
                    nd = nnl ;
                    vlq.push( nnr ) ;
                }
                
            } else {
                if ( vlq.size() == 0 )
                    break ;
                else {
                    nd = vlq.front() ;
                    vlq.pop() ;
                }
            }
        }
    }
    
    
    bool symmetric() {
        
        queue< Node* > left_tree ;
        queue< Node* > right_tree ;
        
        if ( p_tree->left() != 0 and p_tree->right() != 0 ) {
            left_tree.push( p_tree->left() ) ;
            right_tree.push( p_tree->right() ) ;
        } else {
            return false ;
        }
        
        while( left_tree.size() > 0 and right_tree.size() > 0 ) {
            Node* le ;
            Node* re ;
            
            le = left_tree.front() ;
            left_tree.pop() ; 
            
            re = right_tree.front() ;
            right_tree.pop() ;
            
            if ( le != 0 and re != 0 ) {
                if ( le->value() != re->value() ) 
                    return false ;
                else {
                    if ( le->left() != 0 )
                        left_tree.push( le->left() ) ;
                    if ( le->right() != 0 ) 
                        left_tree.push( le->right() ) ;
                    
                    if ( re->right() != 0 ) 
                        right_tree.push( re->right() ) ;
                    if( re->left() != 0 ) 
                        right_tree.push( re->left() ) ;
                }
                
            } else {
                return false ;
            } 
        }
        
        if( left_tree.size() + right_tree.size() > 0 ) 
            return false ;
        else
            return true ;  
    }
    
private:
    
    Node* add_value_rec( Tree* t , Node *node , int value ) {
        
        if ( value <= node->value() and node->left() == 0 )
            return node->set_left( value ) ;
        else if ( value > node->value() and node->right() == 0 )
            return node->set_right( value ) ;
        else if ( value <= node->value() )
            return t->add_value_rec( t , node->left() , value ) ;
        else 
            return t->add_value_rec( t , node->right() , value ) ;
    }
    
    void print_tree_rec( Tree* t , Node *node ) {
        
        if ( node->left() != 0 ) 
            t->print_tree_rec( t , node->left() ) ;
        
        cout << node->value() << " " ;
        
        if ( node->right() != 0 ) 
            t->print_tree_rec( t , node->right() ) ;
    }
    
    void print_tree_struct_rec( Tree* t , Node *node ) {
        
        cout << node->value() ;
        
        if ( node->is_terminal() )
            return ;
        
        cout << "(" ;
        
        if ( node->left() != 0 ) 
            t->print_tree_struct_rec( t , node->left() ) ;
        else 
            cout << "*" ;
        
        cout << "," ;
        
        if ( node->right() != 0 )
            t->print_tree_struct_rec( t , node->right() ) ;
        else
            cout << "*" ;
        
        cout << ")" ;     
    }
    
    Node* search_rec ( Tree *t , Node *node , int value ) {
        
        if ( node == 0 ) 
            return 0 ;
        
        if ( node->value() == value )
            return node ;
        
        if ( value <= node->value() )
            return t->search_rec( t , node->left() , value ) ;
        else 
            return t->search_rec( t , node->right() , value ) ;
    }
    
    Node* p_tree ;
} ;


tree_iterator::tree_iterator( const Tree* t , bool f ) {
    
    p_end = false ;
    
    if ( not f ) { // begin
        p_end = true ;
        return ;
    } 
    
    if ( t->p_tree == 0 ) {
        p_end = true ;
        return ;
    }    
    
    Node* nd = t->p_tree ;
    
    do {
        p_stn.push( nd ) ;
        nd = nd->left() ;
    } while( nd != 0 ) ;
}


int main( void ) {
    
    int arr[] =  {15, -2, 2, -8, 1, 7, 10, 23, 14 , -44, 21, 55, 1, 0 , 21 , 0 };
    vector<int> v (arr, arr + sizeof(arr) / sizeof(int) );
    
    int arrs[] = {15, -2, 2, -8, 1, 7, 12, 23, 14 , -45, 21, 55, 1, 0 , 0 };
    vector<int> vs (arrs, arrs + sizeof(arrs) / sizeof(int) );
        
    Tree tt ;
    
    for ( int i = 0 ; i < v.size() ; i++ ) {
        tt.add_value( v[i] ) ;
    }
    
    tt.print_tree() ;
    cout << "-----" << endl ;
    
    tt.print_tree_struct() ;
    cout << "-----" << endl ;
    
    Node* h = tt.head() ;
    tt.right_rotation( h ) ;
    
    tt.print_tree() ;
    cout << "-----" << endl ;
    
    tt.print_tree_struct() ;
    cout << "-----" << endl ;
    
    h = tt.head() ;
    tt.left_rotation( h ) ;
    
    tt.print_tree() ;
    cout << "-----" << endl ;
    
    tt.print_tree_struct() ;
    cout << "-----" << endl ;
    
    
    for ( int i = 0 ; i < vs.size() ; i++ ) {
        cout << vs[i] << " " << tt.search( vs[i] ) << endl ;
    }
    
    cout << "-----" << endl ;
    cout << "--df---" << endl ;
    cout << "-----" << endl ;
    
    for ( int i = 0 ; i < vs.size() ; i++ ) {
        cout << vs[i] << " " << tt.search_df( vs[i] ) << endl ;
    }
    
    cout << "-----" << endl ;
    cout << "--bf---" << endl ;
    cout << "-----" << endl ;
    
    for ( int i = 0 ; i < vs.size() ; i++ ) {
        cout << vs[i] << " " << tt.search_bf( vs[i] ) << endl ;
    }
    
    cout << "lower: " << tt.lower_depth() << endl ;
    
    Tree ttt ;
    
    int parrs[] = {-1, 0, 0, 1, 1, 3, 5 };
    vector<int> pa (parrs, parrs + sizeof(parrs) / sizeof(int) );
    
    ttt.parent_array( pa ) ;
    ttt.print_tree_struct() ;
    
    cout << "-----" << endl ;
    
    Tree tttt ;
    
    srand(time(NULL)); 
    for ( int i = 0 ; i < 100 ; i++ ) {
        tttt.insert_treap( i ) ;
    }
    
    tttt.print_tree_struct() ;
    cout << "-----" << endl ;
    tttt.print_tree() ;
    cout << "-----" << endl ;
    
    
    for ( int i = 0 ; i < 100 ; i+= 3 ) {
        tttt.remove_treap( rand()%10 ) ;
    
        tttt.print_tree_struct() ;
        cout << "-----" << endl ;
        tttt.print_tree() ;
        cout << "-----" << endl ;
    }
    
    
    cout << "---itr--" << endl ;
    
//     cout << "-----" << endl ;
//         tttt.print_tree() ;
//         cout << "-----" << endl ;
    
     for( int k : tttt ) {
         cout << k << " " ;
     }
    cout << endl <<  "-----" << endl ;
    
}



