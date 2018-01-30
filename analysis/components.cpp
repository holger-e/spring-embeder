#include <bits/stdc++.h>

using namespace std;

vector<vector<int>> adjlist;
vector<string> names;
vector<int> descendants;
map<int, set<int>> years;
map<int, set<string>> countries;

vector<string> split(string &s, string delim) {
  vector<string> result; char *token;
  token = strtok((char*)s.c_str(), (char*)delim.c_str());
  while (token != NULL) {
    result.push_back(string(token));
    token = strtok(NULL, (char*)delim.c_str());
  }
  return result;
}

string findCountry(int v, int n) {
  for (int i = 0; i < n; i++) {
    for (int w : adjlist[i]) {
      if (w == v && !countries[i].empty()) {
        return *(countries[i].begin());
      }
    }
  }

  queue<int> q;
  q.push(v);
  vector<bool> visited(n, false);
  visited[v] = true;

  while (!q.empty()) {
    int v = q.front();
    q.pop();

    for (int w : adjlist[v]) {
      if (!countries[w].empty()) {
        return *(countries[w].begin());
      }
      if (!visited[w]) {
        q.push(w);
      }
    }
  }
  while (true);
  return "";
}

void comp(int num) {
  adjlist.clear();
  names.clear();
  countries.clear();
  years.clear();
  descendants.clear();

  freopen(("../components/comp" + to_string(num) + ".txt").c_str(), "r", stdin);
  freopen(("../components/comp" + to_string(num) + ".in").c_str(), "w", stdout);

  int n, m;
  cin >> n >> m;
  adjlist.resize(n);
  names.resize(n);
  descendants.resize(n);
  getchar();

  for (int i = 0; i < n; i++) {
    getline(cin, names[i]);
    cin >> descendants[i];
    getchar();
  }

  for (int i = 0; i < m; i++) {
    string line;
    getline(cin, line);
    auto parts = split(line, " ");

    int from = atoi(parts[0].c_str());
    int to = atoi(parts[1].c_str());
    int year = atoi(parts[2].c_str());

    adjlist[from].push_back(to);
    years[to].insert(year);
    if (parts.size() == 4) countries[to].insert(parts[3]);
  }

  cout << n << " " << m << endl;
  for (int i = 0; i < n; i++) {
    cout << i << endl;
    cout << names[i] << endl;
    cout << descendants[i] << endl;

    // Guess a year 25 years before average child years.
    if (years[i].empty()) {
      int sum = 0;
      int summands = 0;
      for (int w : adjlist[i]) {
        for (int y : years[w]) {
          sum += y;
          summands++;
        }
      }
      int avg = round((double)sum / summands);
      years[i].insert(avg - 25);
    } else if (years[i].size() >= 2) {
      int sum = 0;
      int summands = 0;
      for (int y : years[i]) {
        sum += y;
        summands++;
      }
      years[i].clear();
      if (summands == 0) cout << "division by zero" << endl;
      years[i].insert(round((double)sum / summands));
    }
    cout << *(years[i].begin()) << endl;

    if (countries[i].empty()) {
      countries[i].insert(findCountry(i, n));
    } else if (countries[i].size() >= 2) {
      while (countries[i].size() >= 2) {
        countries[i].erase(countries[i].begin());
      }
    }
    cout << *(countries[i].begin()) << endl;
  }
  for (int i = 0; i < n; i++) {
    cout << adjlist[i].size();
    for (int w : adjlist[i]) cout << " " << w;
    cout << endl;
  }

  /*for (int i = 0; i < n; i++) {
    if (countries[i].size() >= 2) {
      cout << "Person " << i << " has " << countries[i].size() << " countries:";
      for (string s : countries[i]) cout << " " << s;
      cout << endl;
    } else if (countries[i].size() == 1 && *(countries[i].begin()) == "empty") {
      cout << "Person " << i << " has no countries." << endl;
    }
  }*/

  /*for (int i = 0; i < n; i++) {
    if (years[i].size() >= 2) {
      cout << "Person " << i << " has " << years[i].size() << " years:";
      for (int y : years[i]) cout << " " << y;
      cout << endl;
    }
  }*/
}
  
int main() {
  for (int i = 0; i < 122; i++) {
    comp(i);
  }
  return 0;
}
