#include <bits/stdc++.h>

using namespace std;

string readComponent(int i) {
  string res = "{";
  res += "\"vertices\": [";

  freopen(("comp" + to_string(i) + ".in").c_str(), "r", stdin);
  int n, m;
  cin >> n >> m;

  for (int i = 0; i < n; i++) {
    if (i) res += ", ";

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

    res += "{";
    res += "\"name\": \"" + name + "\"";
    res += ", ";
    res += "\"descendants\": " + to_string(descendants);
    res += ", ";
    res += "\"year\": " + to_string(year);
    res += ", ";
    res += "\"country\": \"" + country + "\"";
    res += "}";
  }
  res += "], \"edges\": [";
  bool first = true;
  for (int i = 0; i < n; i++) {
    int k;
    cin >> k;

    for (int j = 0; j < k; j++) {
      int from = i;
      int to;
      cin >> to;
      if (!first) {
        res += ", ";
      }
      first = false;

      res += "{";
      res += "\"from\": " + to_string(from) + ", \"to\": " + to_string(to);
      res += "}";
    }
  }
  res += "]";

  res += "}";

  freopen(("comp" + to_string(i) + ".js").c_str(), "w", stdout);
  cout << "const COMP_" << to_string(i) << " = " << res << ";" << endl;
  return res;
}

int main() {
  string all = "const GRAPH = ";
  all += "[";
  for (int i = 0; i < 122; i++) {
    if (i) all += ", ";
    all += readComponent(i);
  }
  all += "];";
  freopen("all.js", "w", stdout);
  cout << all << endl;
}
