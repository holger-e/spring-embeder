#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct node{
    string name;
    string country;
    int descendants;
    int year;
};

bool node_comp(node a, node b){
    return a.year < b.year;
}

struct edge{
    int from;
    int to;
};

vector<node> nodes;
vector<edge> edges;

int main(){
    
    int n,m;
    cin >> n;
    cin >> m;
    
    string s;
    getline(cin, s);
    
    //parse nodes
    for (int i = 0; i < n; ++i){
        node n;
        getline(cin,n.name);
        cin >> n.descendants;
        cin >> n.year;
        getline(cin, s);
        getline(cin, n.country);
        nodes.push_back(n);
    }
    
    
    // parse edges
    for (int i = 0; i < m; ++i){
        edge e;
        cin >> e.from >> e.to;
       /* if (e.from == e.to)
            cout << "LOOP" << endl;*/
        edges.push_back(e);
    }   
        

    
    cout << "Creator \"...\"" << endl << "directed 1" << endl << "graph [" << endl;
    
    int i = 0;
    for (node n : nodes){
        cout << "node [" << endl << "id " << i << endl << "template \"oreas:std:rect simple\"" << endl <<
        "label \"" << n.name << " / " << n.country << " / " << n.descendants << " / " << n.year << "\"" << endl;
        
        cout << "graphics [\nw 100.0\nh 23.0\nfill \"#add8e6\"\nline \"#4682b4\"\npattern \"1\"\nstipple 1\nlineWidth 2.0\nwidth 1.0\n]" << endl;
        cout << "]" << endl;
        ++i;
    }
    for (edge e : edges){
        cout << "edge [" << endl << "source " << e.from << endl << "target " << e.to << endl << "generalization 0" << endl;
        cout << "graphics [" << endl << "type \"line\"" << endl  << "arrow \"none\"" << endl << "stipple 1" << endl << "lineWidth 2.000000" << endl << "fill \"#000000\"" << endl << "]" << endl;
        cout << "]" << endl;
    }
    
    cout << "]" << endl;
    
    
    
        // output levels
    /*
    cout << endl << endl;
    sort(nodes.begin(),nodes.end(), node_comp);
    
    int level = 0;
    int current_year = nodes[0].year;
    for (node n : nodes){
        if (n.year != current_year){
            current_year = n.year;
            level++;
        }
        cout << level << endl; //", ";
    }
    
    cout << endl;
    */
    return 0;
}
