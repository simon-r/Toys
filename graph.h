#include<iostream> 
#include<vector>
#include<list>
#include<map>
#include<stack>
#include<queue>
#include<tuple>
#include<cstdlib>
#include<time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>

#ifndef GRAPH_H
#define GRAPH_H

using namespace std ;

class node {
public:
    
    node() {}
    
    node( int id ) : p_id( id ) {} 
    
    void add_edge( int next_node ) {
        p_edges.push_back( next_node ) ;
    }
    
    void add_weighted_edge( int next_node , int w ) {
        p_edges.push_back( next_node ) ;
        p_weights[next_node] = w ;
    }
    
    int weight( int next_node ) {
        return p_weights[next_node] ;
    }
    
    int neighbour_cnt() {
        return p_edges.size() ;
    }
    
    void unique_edges() {
        p_edges.sort() ;
        p_edges.unique() ;
    }
    
    list<int>::iterator edges_begin() {
        return p_edges.begin() ;
    }
    
    list<int>::iterator edges_end() {
        return p_edges.end() ;
    }
    
private:
    
    int p_id ;
    list<int> p_edges ;
    map<int,int> p_weights ;
} ;


class graph {
    
public:
    
    graph() {}
    
    void add_node( int id ) {
        node nnd( id ) ;
        p_graph[id] = nnd ;
    }
    
    node& get_node( int id ) {
        return p_graph[id] ;
    }
    
    int nodes_cnt() {
        return p_graph.size() ;
    }
    
    void clear() {
        p_graph.clear() ;
        p_group.clear() ;
    }
    
    void add_edge( int na , int nb ) {
        
        if ( p_graph.find( na ) == p_graph.end() )
            add_node( na ) ;
        
        p_graph[na].add_edge( nb ) ;
    }
    
    void add_weighted_edge( int na , int nb , int w ) {
        if ( p_graph.find( na ) == p_graph.end() )
            add_node( na ) ;
        
        p_graph[na].add_weighted_edge( nb , w ) ;
    }
    
    int weight( int na , int nb ) {
        return get_node( na ).weight( nb ) ;
    }
    
    graph invert() {
        graph res ;
        
        for ( auto it = p_graph.begin() ; it != p_graph.end() ; ++it ) {
            int dnd = it->first ;
            
            for ( auto ite = edges_begin( it->first ) ; ite != edges_end( it->first ) ; ++ite ) {
                res.add_edge( *ite , dnd ) ;
            }
        }

        return res ;
    }
    
    void add_nodes_group( list<int> &gr ) {
        p_group.push_front( gr ) ;
    }
    
    graph build_scc_equivalent() {
        
        graph res ;
        map< list<int>* , int > lp ;
        map< int , list<int>* > np ;
            
        int i = 1 ;
        for ( auto itr = p_group.begin() ; itr != p_group.end() ; ++itr ) {
            res.add_node( i ) ;
            
            lp[&(*itr)] = i ;
            
            i++ ;
            
            for ( auto itg = itr->begin() ; itg != itr->end() ; ++itg ) {
                np[*itg] = &(*itr) ;
            }
        }
        
        i = 1 ;
        for ( auto itr = p_group.begin() ; itr != p_group.end() ; ++itr ) {
           for ( auto itg = itr->begin() ; itg != itr->end() ; ++itg ) {
               
               for( auto ite = edges_begin( *itg ) ; ite != edges_end( *itg ) ; ++ite ) {
                   if ( np[*ite] != &(*itr) ) {
                       res.add_edge( i , lp[ np[*ite] ] ) ;
                   }
               }
               
           }
           
           i++ ;
        }
        
        for ( auto itr = res.nodes_begin() ; itr != res.nodes_end() ; ++itr ) {
            itr->second.unique_edges() ;
        }
        
        return res ;
    }
    
    list<int>::iterator edges_begin( int nd ) {
        return p_graph[nd].edges_begin() ;
    }
    
    list<int>::iterator edges_end( int nd ) {
        return p_graph[nd].edges_end() ;
    }
    
    map<int,node>::iterator nodes_begin() {
        return p_graph.begin() ;
    }
    
    map<int,node>::iterator nodes_end() {
        return p_graph.end() ;
    }
    
    map<int,node>::reverse_iterator reverse_nodes_begin() {
        return p_graph.rbegin() ;
    }
    
    map<int,node>::reverse_iterator reverse_nodes_end() {
        return p_graph.rend() ;
    }
    
    list< list<int> >::iterator groups_begin() {
        return p_group.begin() ;
    }
    
    list< list<int> >::iterator groups_end() {
        return p_group.end() ;
    }
    
    void print_graph() {
        for ( auto it = p_graph.begin() ; it != p_graph.end() ; ++it ) {
            cout << it->first << " | " ;
            
            for ( auto ite = edges_begin( it->first ) ; ite != edges_end( it->first ) ; ++ite ) {
                cout << *ite << "  " ;
            }
            
            cout << endl ;
        }
    }
    
    void print_wgraph() {
        for ( auto it = p_graph.begin() ; it != p_graph.end() ; ++it ) {
            cout << it->first << " | " ;
            
            for ( auto ite = edges_begin( it->first ) ; ite != edges_end( it->first ) ; ++ite ) {
                cout << *ite << "(" << get_node( it->first ).weight( *ite ) << ")  " ;
            }
            
            cout << endl ;
        }
    }    
    
private:
    
    map<int,node> p_graph ;
    list< list<int> > p_group ;
} ;


list<graph> read_graph_file( string file_name ) {
    
    string line;
    ifstream myfile ( file_name );
    
    list<graph> res ;
    
    if (myfile.is_open()) {
        
        int gr_cnt = 0 ;
        graph gr ;
        bool f = false ;
        
        while ( getline ( myfile , line ) ) {
            //cout << line << '\n';
            vector<string> strs ;
            boost::split(strs, line, boost::is_any_of(" ")) ;
            
            int sz = strs.size() ;
            
            if ( sz > 1 ) {
                if ( f ) 
                    gr.add_edge( stoi( strs[0] ) , stoi( strs[1] ) ) ;
                else {
                    for ( int i = 1 ; i <= stoi( strs[0] ) ; i++ ) {
                        gr.add_node( i ) ;
                    }
                    f = true ;
                }
            } else {
                if ( gr.nodes_cnt() > 0 ) {
                    res.push_back( gr ) ;
                    gr.clear() ;
                    f = false ;
                }
            }
        }
        
        if ( gr.nodes_cnt() > 0 ) {
            res.push_back( gr ) ;
            gr.clear() ;
        }
        
        myfile.close();
        
    } else 
        cout << "Unable to open file"; 
    
    return res ;
}


list<graph> read_w_graph_file( string file_name ) {
    
    string line;
    ifstream myfile ( file_name );
    
    list<graph> res ;
    
    if (myfile.is_open()) {
        
        int gr_cnt = 0 ;
        graph gr ;
        bool f = false ;
        
        while ( getline ( myfile , line ) ) {
            //cout << line << '\n';
            
            vector<string> strs(3) ;
            
            boost::split(strs, line, boost::is_any_of(" ")) ;
            
            int sz = strs.size() ;
            
            if ( sz > 1 ) {
                if ( f ) 
                    gr.add_weighted_edge( stoi( strs[0] ) , stoi( strs[1] ) , stoi( strs[2] ) ) ;
                else {
                    for ( int i = 1 ; i <= stoi( strs[0] ) ; i++ ) {
                        gr.add_node( i ) ;
                    }
                    f = true ;
                }
            } else {
                if ( gr.nodes_cnt() > 0 ) {
                    res.push_back( gr ) ;
                    gr.clear() ;
                    f = false ;
                }
            }
        }
        
        if ( gr.nodes_cnt() > 0 ) {
            res.push_back( gr ) ;
            gr.clear() ;
        }
        
        myfile.close();
        
    } else 
        cout << "Unable to open file"; 
    
    return res ;
}





list<graph> read_2sat_file( string file_name ) {
    
    string line;
    ifstream myfile ( file_name );
    
    list<graph> res ;
    
    if (myfile.is_open()) {
        
        int gr_cnt = 0 ;
        graph gr ;
        bool f = false ;
        
        while ( getline ( myfile , line ) ) {
            //cout << line << '\n';
            vector<string> strs ;
            boost::split(strs, line, boost::is_any_of(" ")) ;
            
            int sz = strs.size() ;
            
            if ( sz > 1 ) {
                if ( f ) {
                    gr.add_edge( -1*stoi( strs[0] ) , stoi( strs[1] ) ) ;
                    gr.add_edge( -1*stoi( strs[1] ) , stoi( strs[0] ) ) ;
                } else {
                    for ( int i = 1 ; i <= stoi( strs[0] ) ; i++ ) {
                        gr.add_node( i ) ;
                        gr.add_node( -i ) ;
                    }
                    f = true ;
                }
            } else {
                if ( gr.nodes_cnt() > 0 ) {
                    res.push_back( gr ) ;
                    gr.clear() ;
                    f = false ;
                }
            }
        }
        
        if ( gr.nodes_cnt() > 0 ) {
            res.push_back( gr ) ;
            gr.clear() ;
        }
        
        myfile.close();
        
    } else 
        cout << "Unable to open file"; 
    
    return res ;
}


list<graph> read_2sat_file_test( string file_name ) {
    
    string line;
    ifstream myfile ( file_name );
    
    list<graph> res ;
    
    if (myfile.is_open()) {
        
        int gr_cnt = 0 ;
        graph gr ;
        bool f = false ;
        
        while ( getline ( myfile , line ) ) {
            //cout << line << '\n';
            vector<string> strs ;
            boost::split(strs, line, boost::is_any_of(" ")) ;
            
            int sz = strs.size() ;
            
            if ( sz > 1 ) {
                if ( f ) {
                    gr.add_edge( -1*stoi( strs[0] ) , stoi( strs[1] ) ) ;
                } else {                   
                    f = true ;
                }
            } else {
                if ( gr.nodes_cnt() > 0 ) {
                    res.push_back( gr ) ;
                    gr.clear() ;
                    f = false ;
                }
            }
        }
        
        if ( gr.nodes_cnt() > 0 ) {
            res.push_back( gr ) ;
            gr.clear() ;
        }
        
        myfile.close();
        
    } else 
        cout << "Unable to open file"; 
    
    return res ;
}

#endif