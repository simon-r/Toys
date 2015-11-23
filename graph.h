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
#include <boost/regex.hpp>


#ifndef GRAPH_H
#define GRAPH_H

// g++ xxx.cpp -o xxx -lboost_system -lboost_regex -std=c++14

using namespace std ;

class node {
public:
    
    node() {}
    
    node( int id ) : p_id( id ) {} 
    
    void add_edge( int next_node ) {
        p_edges.push_back( next_node ) ;
        
        pm_edges[next_node] = true ;
    }
    
    bool edge_exists( int next_node ) {
        if ( pm_edges.find( next_node ) == pm_edges.end() )
            return false ;
        else
            return true ;
    }
    
    void add_weighted_edge( int next_node , int w ) {
        add_edge ( next_node ) ;
        set_weight( next_node , w ) ;
    }
    
    void set_weight( int next_node , int w ) {
        p_weights[next_node] = w ;
    }
    
    int weight( int next_node ) {
        return p_weights[next_node] ;
    }
    
    int neighbour_cnt() {
        return pm_edges.size() ;
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
    
    map<int,bool> pm_edges ;
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
        
        if ( p_graph.find( nb ) == p_graph.end() )
            add_node( nb ) ;
        
        p_graph[na].add_edge( nb ) ;
    }
    
    void add_weighted_edge( int na , int nb , int w ) {
        add_edge( na , nb ) ;
        p_graph[na].set_weight( nb , w ) ;
    }
    
    void set_weight(  int na , int nb , int w ) {
        p_graph[na].set_weight( nb , w ) ;
    }
    
    int weight( int na , int nb ) {
        return get_node( na ).weight( nb ) ;
    }
    
    bool edge_exists( int na , int nb ) {
        return get_node( na ).edge_exists( nb ) ;
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




bool read_graph_file_adw( string file_name , list<graph> &res , int &origin , int &sink ) {
    
    string line;
    ifstream myfile ( file_name );
    
    res.clear() ;
    
    if (myfile.is_open()) {
        
        int gr_cnt = 0 ;
        graph gr ;
        bool f = false ;
        
        int lcnt = 0 ;
        
        while ( getline ( myfile , line ) ) {
            //             vector<string> strs ;
            //             boost::split(strs, line, boost::is_any_of(" ")) ;
            
            string pattern = string( "(\\d+)" );
            //             string pattern = string( "(\\d+)\\s*\\->\\s*(\\d+)(,(\\d+)){0,}" );
            
            boost::regex ip_regex(pattern);
            
            boost::sregex_iterator it(line.begin(), line.end(), ip_regex);
            boost::sregex_iterator end;
            
            int i = 0 ;
            int no , ns , w ;
            
            list<string> matchs ;
            
            for (; it != end; ++it) {    
                matchs.push_back( it->str() ) ;                
            }
            
            if( lcnt < 2 and matchs.size() == 1 ) {
                
                if ( lcnt == 0 ) 
                    origin = stoi( *matchs.begin() ) ;
                
                if ( lcnt == 1 ) 
                    sink = stoi( *matchs.begin() ) ;
                
            } else {
                
                auto itr = matchs.begin() ;
                for ( ; itr != matchs.end() ; ) {
                    
                    if ( i == 0 ) {
                        no = stoi( *itr ) ;
                        ++itr ; 
                    } else {
                        ns = stoi( *itr ) ;
                        ++itr ;
                        w = stoi( (*itr) ) ;
                        ++itr ;
                        gr.add_weighted_edge( no , ns , w ) ;
                    }
                    i++ ;  
                }
                
            }
            
            //cout << endl ;
            lcnt++ ;
        }
        
        res.push_back( gr ) ;
    } else {
        cout << "Unable to open file"; 
        return false ;
    }
    
    return true ;
}



list<graph> read_graph_file_ad( string file_name ) {
    
    string line;
    ifstream myfile ( file_name );
    
    list<graph> res ;
    
    if (myfile.is_open()) {
        
        int gr_cnt = 0 ;
        graph gr ;
        bool f = false ;
        
        while ( getline ( myfile , line ) ) {
            //             vector<string> strs ;
            //             boost::split(strs, line, boost::is_any_of(" ")) ;
            
            string pattern = string( "(\\d+)" );
            //             string pattern = string( "(\\d+)\\s*\\->\\s*(\\d+)(,(\\d+)){0,}" );
            
            boost::regex ip_regex(pattern);
            
            boost::sregex_iterator it(line.begin(), line.end(), ip_regex);
            boost::sregex_iterator end;
            
            int i = 0 ;
            int no , ns ;
            
            for (; it != end; ++it) {
                
                //cout << it->str() << " ";
                
                if ( i == 0 ) {
                    no = stoi( it->str() ) ;
                } else {
                    ns = stoi( it->str() ) ;
                    
                    gr.add_edge( no , ns ) ;
                }
                i++ ;
            }
            
            //cout << endl ;
        }
        
        res.push_back( gr ) ;
    } else {
        cout << "Unable to open file"; 
    }
    
    return res ;
}

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