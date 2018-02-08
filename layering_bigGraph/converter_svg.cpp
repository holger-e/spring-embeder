#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;



struct node{
    string name;
    string country;
    int descendants;
    int year;
    pair<int,int> coord;
};

bool node_comp(node a, node b){
    return a.year < b.year;
}

struct edge{
    int from;
    int to;
    vector<pair<int,int>> bends;
};

vector<node> nodes;
vector<edge> edges;

int n,m;

int main(){
    
    cin >> n >> m;
    
    //parse nodes
    for (int i = 0; i < n; ++i){        
        node n;
      
        string s;
        getline(cin, s);
        getline(cin, s);
        n.name = s;
        cin >> n.descendants;
        cin >> n.year;
        getline(cin, s);
        getline(cin, s);
        n.country = s;
        cin >> n.coord.first;
        cin >> n.coord.second;
        
        nodes.push_back(n);
    }
    

    // parse edges
    for (int i = 0; i < m; ++i){
        edge e;
        cin >> e.from >> e.to;
        int c;
        cin >> c;
        for (int j = 0; j < c; ++j){
            pair<int,int> p;
            cin >> p.first >> p.second;
            e.bends.push_back(p);
        }
        edges.push_back(e);
    }
   
   
   
   // export everything to svg
   cout << nodes.size() << " " << edges.size() << endl;
   // export nodes
   for (node n : nodes){
       cout << n.name << endl;
       cout << n.descendants << endl;
       cout << n.year << endl;
       cout << n.country << endl;
       cout << n.coord.first << endl;
       cout << n.coord.second << endl;
   }
   
   // export edges
   for (edge e: edges){
        cout << e.from << " " << e.to << " " << e.bends.size();
        
        for (pair<int,int> p : e.bends){
            cout << " " << p.first << " " << p.second;
        }
        cout << endl;
   }
   
    return 0;
}
