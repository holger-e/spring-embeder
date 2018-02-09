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

bool nodes_x(node a, node b){
	return a.coord.first > a.coord.first;
}

bool nodes_y(node a, node b){
	return a.coord.second > a.coord.second;
}

bool node_comp(node a, node b){
    return a.year < b.year;
}

bool deg_comp(pair<int,pair<int,int>> a, pair<int,pair<int,int>> b){
	return a.first < b.first;
}

bool deg_year(pair<int,pair<int,int>> a, pair<int,pair<int,int>> b){
	return a.second.second < b.second.second;
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
    
    int x = 0;
    int y = 0;
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
        
        if (x < n.coord.first)
			x = n.coord.first;
		if (y < n.coord.second)
			y = n.coord.second;
        
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

    cout << "max x: " << x << endl;
   
   cout << "max y: " << y << endl;
   /*
   // calc degree
   vector<pair<int,pair<int,int>>> deg; // x-coord, degree, year
   for (int i = 0; i < n; ++i){
	   deg.push_back(pair<int,pair<int,int>>(nodes[i].coord.first,pair<int,int>(0,nodes[i].year)));
   }
	for (edge e : edges){
		deg[e.from].second.first++;
		deg[e.to].second.first++;
   }
   
   
   
   sort(deg.begin(), deg.end(), deg_comp);
   
   auto h = deg.begin();
   
   for (pair<int,pair<int,int>> p : deg){
	   cout << "x: " << p.first << " deg: " << p.second.first << " year: " << p.second.second << endl;
	   if (p.first < 100000){
		h++;   
	   }
   }
   
   cout << "---------------------------" << endl;
   
   sort(h, deg.end(), deg_year);
   
   while (h != deg.end()){
	   pair<int,pair<int,int>> p = *h;
	   cout << "x: " << p.first << " deg: " << p.second.first << " year: " << p.second.second << endl;
	   h++;
   }
   */
   /*
   
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
    
   */
   
    return 0;
}
