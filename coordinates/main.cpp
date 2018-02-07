#include <bits/stdc++.h>

// Types for vertices.
#define MATHEMATICIAN 0
#define DUMMY 1

// The height of a year in the drawing.
#define PIXELS_PER_YEAR 20

// The year at the top of the drawing.
#define MIN_YEAR 1300

// The x-difference between two vertices on the same layer.
#define X_DIFFERENCE 200

// How often to reorder the vertices on each level.
// Actually twice as many reorderings happen. One top-down, one bottom-up.
#define REORDER_OUTER_ITERATIONS 1
#define REORDER_INNER_ITERATIONS 5

// How often to reorder randomly.
#define REORDER_RANDOM_ITERATIONS 1000000

using namespace std;

typedef pair<int, int> ii;

struct vert {
  int type;
  int idx;
  int descendants;
  string name;
  int year;
  string country;
  int x;
  int y;
};

struct edge {
  int from;
  int to;
};

int n, m;
vector<vert> verts;
vector<edge> edges; 
vector<vector<int>> adjlist, inverse;
map<int, vector<int>> levels;

void assignYCoordinates() {
  for (auto &v : verts) {
    v.y = (v.year - MIN_YEAR) * PIXELS_PER_YEAR;
  }
}

void addDummyVertices() {
  for (auto &e : edges) {
    int yearFrom = verts[e.from].year;
    int yearTo = verts[e.to].year;

    int fromIdx = e.from;
    for (int year = yearFrom + 1; year < yearTo; year++) {
      verts.push_back(vert {
        DUMMY,
        (int)verts.size(),
        0,
        "",
        year,
        "",
        0,
        0
      });
      adjlist.push_back(vector<int>());
      inverse.push_back(vector<int>());
      int toIdx = verts.size() - 1;
      adjlist[fromIdx].push_back(toIdx);
      inverse[toIdx].push_back(fromIdx);
      fromIdx = toIdx;
    }
    inverse[e.to].push_back(fromIdx);
    adjlist[fromIdx].push_back(e.to);
  }
}

void assignToLevels() {
  for (auto &v : verts) {
    levels[v.y].push_back(v.idx);
  }
}

void assignRandomXCoordinates() {
  srand(time(NULL));
  for (auto &l : levels) {
    set<int> xCoords;
    vector<int> &vec = l.second;
    for (auto &v : vec) {
      int x = rand();
      while (xCoords.find(x) != xCoords.end()) x = rand();
      verts[v].x = x;
      xCoords.insert(x);
    }
  }
}

void assignGreedyXCoordinates() {
  for (auto &l : levels) {
    vector<int> &v = l.second;
    sort(v.begin(), v.end(), [](int a, int b){
      return verts[a].x < verts[b].x;
    });
    for (int i = 0; i < (int)v.size(); i++) {
      verts[v[i]].x = i * X_DIFFERENCE;
    }
  }
}

void reorderBottomUp() {
  for (auto l = next(levels.begin()); l != levels.end(); l++) {
    set<int> xCoords;
    for (auto &v : l->second) {
      int sum = 0;
      int degree = 0;
      for (int w : inverse[v]) {
        sum += verts[w].x;
        degree++;
      }
      int x = verts[v].x;
      if (degree) x = round(sum / (double)degree);
      while (xCoords.find(x) != xCoords.end()) x++;
      verts[v].x = x;
      xCoords.insert(x);
    }
  }
}

void reorderTopDown() {
  for (auto l = levels.begin(); l != prev(levels.end()); l++) {
    set<int> xCoords;
    for (auto &v : l->second) {
      int sum = 0;
      int degree = 0;
      for (int w : adjlist[v]) {
        sum += verts[w].x;
        degree++;
      }
      int x = verts[v].x;
      if (degree) x = round(sum / (double)degree);
      while (xCoords.find(x) != xCoords.end()) x++;
      verts[v].x = x;
      xCoords.insert(x);
    }
  }
}

int countCrossingsOnLevel(int l) {
  if (l == (int)levels.size() || l < 0) return 0;
  auto lower = levels.begin();
  for (int i = 0; i < l; i++) lower = next(lower);

  int result = 0;
  for (int l1 : lower->second) {
    for (int u1 : adjlist[l1]) {
      // There is an edge from l1 to u1 on this layer.
      for (int l2 : lower->second) {
        if (l1 == l2) continue;
        for (int u2 : adjlist[l2]) {
          // There is another edge from l2 to u2 on this layer.
          if ((verts[l1].x < verts[l2].x && verts[u1].x > verts[u2].x) ||
              (verts[l1].x > verts[l2].x && verts[u1].x < verts[u2].x)) {
            result++;
          }
        }
      }
    }
  }
  // Result is actually twice the number of crossings, since every intersection
  // is counted by both edges.
  return result / 2;
}

int countCrossings() {
  int result = 0;
  for (int i = 0; i < (int)levels.size() - 1; i++) {
    result += countCrossingsOnLevel(i);
  }
  return result;
}

void reoroderRandom() {
  int numLevels = levels.size();
  int crossings = countCrossings();
  for (int counter = 0; counter < REORDER_RANDOM_ITERATIONS; counter++) {
    if (counter % 1000 == 0) {
      cout << "iteration " << counter << endl;
    }
    int l = rand() % numLevels;

    auto level = levels.begin();
    for (int i = 0; i < l; i++) level = next(level);

    int size = level->second.size();
    if (size < 2) continue;

    int idx1, idx2;
    do {
      idx1 = rand() % size;
      idx2 = rand() % size;
    } while (idx1 == idx2);

    int v1 = level->second[idx1];
    int v2 = level->second[idx2];
    int before = countCrossingsOnLevel(l) + countCrossingsOnLevel(l - 1);
    // int totalBefore = countCrossings();
    swap(verts[v1].x, verts[v2].x);
    int after = countCrossingsOnLevel(l) + countCrossingsOnLevel(l - 1);
    // int totalAfter = countCrossings();
    // int totalDiff = totalBefore - totalAfter;
    // int layerDiff = before - after;
    // if (totalDiff != layerDiff) {
    //   cout << "Error" << endl;
    // }
    if (before < after) swap(verts[v1].x, verts[v2].x);
    else {
      crossings -= before - after;
      cout << "Crossings: " << crossings << endl;
    }
  }
}

void output(int num) {
  freopen(("../components/comp" + to_string(num) + ".js").c_str(), "w", stdout);

  cout << "const COMP_" << num << " = {" << endl;
  cout << "  \"vertices\": [" << endl;
  for (int i = 0; i < (int)verts.size(); i++) {
    cout << "    {" << endl;
    cout << "      \"type\": " << verts[i].type << "," << endl;
    cout << "      \"name\": \"" << verts[i].name << "\"," << endl;
    cout << "      \"descendants\": " << verts[i].descendants << "," << endl;
    cout << "      \"year\": " << verts[i].year << "," << endl;
    cout << "      \"country\": \"" << verts[i].country << "\"," << endl;
    cout << "      \"x\": " << verts[i].x << "," << endl;
    cout << "      \"y\": " << verts[i].y << endl;
    cout << "    }";
    if (i < (int)verts.size() - 1) cout << ",";
    cout << endl;
  }
  cout << "  ]," << endl;
  cout << "  \"edges\": [" << endl;
  for (int i = 0; i < (int)adjlist.size(); i++) {
    for (int j = 0; j < (int)adjlist[i].size(); j++) {
      cout << "    {\"from\": " << i << ", \"to\": " << adjlist[i][j] << "}";
      if (j < (int)adjlist[i].size() - 1 ||
          i < (int)adjlist.size() - 1) cout << ",";
      cout << endl;
    }
  }
  cout << "  ]" << endl;
  cout << "};" << endl;
}

void comp(int num) {
  freopen(("../components/comp" + to_string(num) + ".in").c_str(), "r", stdin);

  cin >> n >> m;
  verts.resize(n);
  adjlist.resize(n);
  inverse.resize(n);

  // Reads vertices.
  for (int i = 0; i < n; i++) {
    verts[i].type = MATHEMATICIAN;
    cin >> verts[i].idx;
    getchar();
    getline(cin, verts[i].name);
    cin >> verts[i].descendants;
    cin >> verts[i].year;
    cin >> verts[i].country;
    verts[i].x = 0;
    verts[i].y = 0;
  }

  // Reads edges.
  for (int from = 0; from < n; from++) {
    int c;
    cin >> c;
    for (int j = 0; j < c; j++) {
      int to;
      cin >> to;
      edges.push_back(edge {from, to});
    }
  }

  addDummyVertices();
  assignYCoordinates();
  assignToLevels();
  for (int j = 0; j < REORDER_OUTER_ITERATIONS; j++) {
    cout << "Outer iteration " << j << ":" << endl;
    assignRandomXCoordinates();

    for (int i = 0; i < REORDER_INNER_ITERATIONS; i++) {
      reorderBottomUp();
      assignGreedyXCoordinates();
      reorderTopDown();
      assignGreedyXCoordinates();
      cout << "Crossings (after " << i << " inner iterations): "
           << countCrossings() << endl;
    }
  }
  // assignRandomXCoordinates();
  reoroderRandom();
  output(num);
}

int main() {
  comp(0);
  return 0;
}
