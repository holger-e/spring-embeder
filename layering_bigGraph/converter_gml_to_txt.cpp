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

int main(){
    
    string text;
    getline(cin,text);
    getline(cin,text);
    getline(cin,text);
    //parse nodes
    
    int n = 2277;
    for (int i = 0; i < n; ++i){        
        node n;

        getline(cin,text); // node
        getline(cin,text); // id
        getline(cin,text); // template
        getline(cin,text); // label
        
        int j = text.find_first_of("\"", 0);
        int k = text.find_first_of("\"", j+1);
        
        string s = text.substr(j+1, k-j-1);
        j = s.find_first_of("/",0);
        n.name = s.substr(0,j-1);
        k = s.find_first_of("/",j+3);
        n.country = s.substr(j+2,k-j-3);
        j = k + 2;
        //cout << s.substr(j,s.length() - j) << endl;
        n.descendants = stoi(s.substr(j,s.length() - j));
        //cout << "--" << n.descendants << "--" << endl;
        
        getline(cin,text); // graphics
        getline(cin,text); // x
        float x = stof(text.substr(8,text.length()));
        //cout << x << endl;
        getline(cin,text); // y
        float y = stof(text.substr(8,text.length()));
        n.coord.first = (int) floor(x + 0.5);
        n.coord.second = (int) floor(y + 0.5);
        getline(cin,text); // w
        getline(cin,text); // h
        getline(cin,text); // fill
        getline(cin,text); // line
        getline(cin,text); // pattern
        getline(cin,text); // stipple
        getline(cin,text); // lineWidth
        getline(cin,text); // type
        getline(cin,text); // ]
        getline(cin,text); // ]        
        
        nodes.push_back(n);
    }
    
    
    // parse edges
   /* while(getline(cin,text)){
        edge e;
        getline(cin,text);
        getline(cin,text);
        
        getline(cin,text); // graphics
        getline(cin,text); // type
        getline(cin,text); // arrow
        
        edges.push_back(e);
    }*/
   
   // export for voronoi
   
   vector<string> country;
   
   cout << nodes.size() << endl;
    for (node n : nodes){
        int id = 0;
        for (; id < country.size(); ++id){
            if (country[id].compare(n.country) == 0) break;
        }
        if (id == country.size()){
            country.push_back(n.country);
        }
        cout << n.coord.first << " " << n.coord.second << " " << id << endl;
    }
   
    return 0;
}
