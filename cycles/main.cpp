#include <bits/stdc++.h>

#define INF 0x3FFFFFFF

using namespace std;

// A mathematician, meaning a vertex in the graph.
struct vertex {
  int id; // A consecutive id.
  int inputId; // The id from the input.
  int descendants; // The total number of descendants, does not match #children.
  string name; // The name.
};

// A advisor-advisee relation, meaning an edge in the graph.
struct edge {
  int advisorId, adviseeId; // Both using the consecutive form.
  int year; // Year of publication.
  string country; // Country of publication or empty.
};

// Maps the given id to a consecutive id.
map<int, int> idx;

// All the contained mathematicians.
vector<vertex> mathematicians;

// Two graphs, one directed and one undirected to test connectivity.
vector<vector<edge>> directed, undirected;

// Stores indegree and outdegree of each vertex in the directed graph.
vector<int> indeg, outdeg;

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

// Depth first search to explore graph and count size of subgraph.
int visitCon(int v, vector<vector<edge>> &adjlist, vector<bool> &visited) {
  visited[v] = true;
  int size = 1;
  for (auto w : adjlist[v]) {
    if (!visited[w.adviseeId]) {
      size += visitCon(w.adviseeId, adjlist, visited);
    }
  }
  return size;
}

int main() {
  freopen("../math-graph.txt", "r", stdin);

  // Read number of vertices.
  string line;
  getline(cin, line);
  int n = atoi(line.c_str());

  // Resizing the two graph.
  directed.resize(n);
  undirected.resize(n);
  indeg.resize(n);
  outdeg.resize(n);

  // Reads each vertex.
  for (int i = 0; i < n; i++) {
    getline(cin, line);
    vector<string> parts = split(line, " ");

    int id = atoi(parts[0].c_str());
    int descendants = atoi(parts[1].c_str());
    string name = "";
    for (int i = 2; i < (int)parts.size(); i++) {
      if (i > 2) name += " ";
      name += parts[i];
    }

    mathematicians.push_back(vertex {i, id, descendants, name});
    idx[id] = i;
  }

  // Reads number of edges.
  getline(cin, line);
  int m = atoi(line.c_str());

  // Reads edges.
  for (int i = 0; i < m; i++) {
    getline(cin, line);
    vector<string> parts = split(line, " ");
    int advisorId = idx[atoi(parts[0].c_str())];
    int adviseeId = idx[atoi(parts[1].c_str())];
    int year = atoi(parts[2].c_str());
    string country = "";
    if (parts.size() == 4) {
      country = parts[3];
    }

    directed[advisorId].push_back(edge {advisorId, adviseeId, year, country});
    indeg[adviseeId]++;
    outdeg[advisorId]++;

    undirected[advisorId].push_back(edge {advisorId, adviseeId, year, country});
    undirected[adviseeId].push_back(edge {adviseeId, advisorId, year, country});
  }

  
  // Connectivity.
  vector<bool> visited(n, false);
  int components = 0;
  map<int, int> componentSizes;
  for (int i = 0; i < n; i++) {
    if (!visited[i]) {
      components++;
      int size = visitCon(i, undirected, visited);
      componentSizes[size]++;
    }
  }
  cout << "The graph has " << components
       << " components of the following sizes:" << endl;
  for (auto e : componentSizes) {
    cout << "  " << e.first << ": " << e.second << endl;
  }
  cout << endl;

  // Cycles.
  queue<int> q;
  vector<int> indeg2 = indeg;
  for (int i = 0; i < n; i++) {
    if (indeg2[i] == 0) q.push(i);
  }
  int counter = 0;
  while (!q.empty()) {
    int v = q.front(); q.pop();
    counter++;

    for (auto w : directed[v]) {
      indeg2[w.adviseeId]--;
      if (indeg2[w.adviseeId] == 0) q.push(w.adviseeId);
    }
  }
  if (counter == n) cout << "There are no cycles. :)" << endl;
  else cout << "There are cycles. :(" << endl;
  cout << endl;

  // Sources and sinks.
  int sources = 0, sinks = 0;
  for (int i = 0; i < n; i++) {
    if (indeg[i] == 0) sources++;
    if (outdeg[i] == 0) sinks++;
  }
  cout << "There are " << sources << " sources and " << sinks << " sinks."
       << endl << endl;

  // Degree sequences.
  map<int, int> in, out;
  for (int i = 0; i < n; i++) {
    in[indeg[i]]++;
    out[outdeg[i]]++;
  }
  cout << "Indegrees:" << endl;
  for (auto e : in) {
    cout << "  " << e.first << ": " << e.second << endl;
  }
  cout << "Outdegrees:" << endl;
  for (auto e : out) {
    cout << "  " << e.first << ": " << e.second << endl;
  }
  cout << endl;

  // Years.
  map<int, int> years;
  for (int i = 0; i < n; i++) {
    for (auto w : directed[i]) {
      years[w.year]++;
    }
  }
  cout << "There are " << years.size() << " year numbers." << endl;
  for (auto e : years) {
    if (e.second == 1) {
      cout << e.first;
    } else {
      cout << e.first << "(" << e.second << ")";
    }
    cout << " ";
  }
  cout << endl << endl;

  // Countries.
  map<string, int> countries;
  for (int i = 0; i < n; i++) {
    for (auto w : directed[i]) {
      countries[w.country]++;
    }
  }
  cout << "There are " << countries.size() - 1 << " countries." << endl;
  for (auto e : countries) {
    if (e.first.size() == 0) {
      cout << "  empty: " << e.second << endl;
    } else cout << "  " << e.first << ": " << e.second << endl;
  }

  return 0;
}
