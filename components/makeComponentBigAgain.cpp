#include <bits/stdc++.h>

#define NUM_COMPONENTS 122

using namespace std;

struct vertex {
  string name;
  int descendants;
  int year;
  string country;
};

vector<vertex> verts;
vector<vector<int>> adjlist;

int main() {
  int offset = 0;
  int edgeCount = 0;
  int vertCount = 0;

  for (int i = 0; i < NUM_COMPONENTS; i++) {
    freopen(("comp" + to_string(i) + ".in").c_str(), "r", stdin);
    int n, m;
    cin >> n >> m;
    edgeCount += m;
    vertCount += n;

    for (int j = 0; j < n; j++) {
      int idx;
      cin >> idx;
      getchar();
      string name;
      getline(cin, name);
      int descendants;
      cin >> descendants;
      int year;
      cin >> year;
      string country;
      cin >> country;

      verts.push_back(vertex {name, descendants, year, country});
      adjlist.push_back(vector<int>());
    }

    for (int j = 0; j < n; j++) {
      int k;
      cin >> k;
      for (int l = 0; l < k; l++) {
        int to;
        cin >> to;
        adjlist[j + offset].push_back(to + offset);
      }
    }

    offset += n;
  }

  freopen("bigGraph.txt", "w", stdout);
  cout << adjlist.size() << " " << edgeCount << endl;
  for (auto v : verts) {
    cout << v.name << endl;
    cout << v.descendants << endl;
    cout << v.year << endl;
    cout << v.country << endl;
  }
  for (int i = 0; i < (int)adjlist.size(); i++) {
    for (int w : adjlist[i]) {
      cout << i << " " << w << endl;
    }
  }
}
