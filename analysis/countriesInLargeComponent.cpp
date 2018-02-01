// Pipe one of the components as input to get details about the country
// relationships.

#include <bits/stdc++.h>

using namespace std;

struct edge {
  int from, to;
};

vector<vector<edge>> adjlist;
vector<string> names;
vector<int> years, lands;
// 23 different countries, including empty.
int graph[23][23];

int countryIdx;
map<string, int> countryMap;
map<string, int> countryCounter;
map<int, string> invCountry;

int idx(string c) {
  if (countryMap.find(c) == countryMap.end()) {
    invCountry[countryIdx] = c;
    countryMap[c] = countryIdx++;
  }
  return countryMap[c];
}

// Splits at all characters from delim.
vector<string> split(string &s, string delim) {
  vector<string> result; char *token;
  token = strtok((char*)s.c_str(), (char*)delim.c_str());
  while (token != NULL) {
    result.push_back(string(token));
    token = strtok(NULL, (char*)delim.c_str());
  }
  return result;
}

using namespace std;
int main() {
  int n, m;
  cin >> n >> m;
  adjlist.resize(n);
  names.resize(n);
  years.resize(n);
  lands.resize(n, -1);
  getchar();

  idx("empty");

  for (int i = 0; i < n; i++) {
    int descendants;
    string name;
    getline(cin, name);
    cin >> descendants;
    getchar();
    names[i] = name;
  }

  for (int i = 0; i < m; i++) {
    int from, to, year;
    string line, country;
    getline(cin, line);

    auto parts = split(line, " ");
    from = atoi(parts[0].c_str());
    to = atoi(parts[1].c_str());
    year = atoi(parts[2].c_str());
    country = parts.size() == 4 ? parts[3] : "empty";
    countryCounter[country]++;
    
    adjlist[from].push_back(edge {from, to});

    int cidx = idx(country);
    years[to] = year;
    if (lands[to] != -1 && lands[to] != cidx) {
      cout << "Someone has two countries." << endl;
    }
    if (cidx != idx("empty")) lands[to] = cidx;
  }

  for (int i = 0; i < n; i++) {
    if (lands[i] == -1) lands[i] = idx("empty");
  }

  cout << "Countries:" << endl;
  for (auto e : countryCounter) {
    cout << e.first << ": " << e.second << endl;
  }
  cout << endl;

  memset(graph, 0, sizeof(graph));
  for (int v = 0; v < n; v++) {
    for (auto w : adjlist[v]) {
      if (lands[v] != lands[w.to]) {
        graph[lands[v]][lands[w.to]] = 1;
      }
    }
  }

  cout << "Edges in country graph:" << endl;
  for (int i = 0; i < countryIdx; i++) {
    for (int j = 0; j < countryIdx; j++) {
      if (graph[i][j]) {
        cout << invCountry[i] << " --> " << invCountry[j] << endl;
      }
    }
  }

  return 0;
}
