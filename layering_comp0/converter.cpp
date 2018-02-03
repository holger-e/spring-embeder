#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;



struct node{
    string name;
    string country;
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
    
    string text;
    getline(cin,text);
    //parse nodes
    
    int index = 0;
    while(text.length() > index){
        
        node n;
        int i = text.find_first_of("{", index);
        
        //cout << i << endl;
        if (i >= text.length()) break;
        int j = text.find_first_of("\"", i + 10);
        n.name = text.substr(i+10, j - i - 10);
        //cout << n.name << endl;
        i = j + 18;
        j = text.find_first_of("\"year\":", i);
        n.year = stoi(text.substr(j + 8, 4));
        //cout << n.year << endl;
        i = j + 12;
        j = text.find_first_of("\"country\":", i);
        i = text.find_first_of("\"", j + 12);
        n.country = text.substr(j + 12, i - j - 12);
        //cout << n.country << endl;
        
        index = i + 2;
        nodes.push_back(n);
    }
    
    getline(cin,text);
    getline(cin,text);
    
    // parse edges
    index = 0;
    while(text.length() > index){
        
        edge e;
        int i = text.find_first_of("{", index);

        if (i >= text.length()) break;
        int j = text.find_first_of(",", i + 9);
        e.from = stoi(text.substr(i+9, j - i - 9));
        //cout << e.from << endl;
        i = text.find_first_of("}", i);
        e.to = stoi(text.substr(j + 8, i - j -8));
        //cout << e.to << endl;
        
        index = i + 2;
        edges.push_back(e);
    }
    
    
    cout << "Creator \"...\"" << endl << "directed 1" << endl << "graph [" << endl;
    
    int i = 0;
    for (node n : nodes){
        cout << "node [" << endl << "id " << i << endl << "template \"oreas:std:rect simple\"" << endl <<
        "label \"" << n.name << " - " << n.country << "\"" << endl;
        
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
    
    /*cout << endl << endl;
    sort(nodes.begin(),nodes.end(), node_comp);
    
    int level = 0;
    int current_year = nodes[0].year;
    for (node n : nodes){
        if (n.year != current_year){
            current_year = n.year;
            level++;
        }
        cout << level << ", ";
    }
    
    cout << endl;*/
    
    return 0;
}
