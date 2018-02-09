#include <bits/stdc++.h>

// Shortcuts to get coordinates of point represented as a complex number.
#define x real()
#define y imag()

// Dimensions of the SVG.
#define SVG_WIDTH 66500
#define SVG_HEIGHT 73000

// Path to the file containing only the coordinates of the vertices and country
// identifiers.
#define VORONOI_TEST_FILE "test_coordinates.txt"
#define VORONOI_FILE "../layering_bigGraph/bigGraph_for_voronoi.txt"

// Path to the file containing the complete foreground drawing information.
#define GRAPH_TEST_FILE "test_everything.txt"
#define GRAPH_FILE "../layering_bigGraph/bigGraph_complete_layout.txt"

// Name of the SVG that the drawing is exported in.
#define OUTPUT_FILE "graph.svg"

// Epsilon value for counter clockwise function.
#define EPS 1e-6

using namespace std;

// Typedefs for geometry library.
typedef complex<double> PT;
typedef vector<PT> POLY;
typedef vector<PT> L;

// Information corresponding to a vertex.
struct vertex {
  string name;
  int descendants;
  int year;
  string country;
  double cx;
  double cy; 
};

// All mathematicians.
vector<vertex> mathematicians;

// All edges. Each edge is a vector of control points.
vector<vector<PT>> edges;

// List with all polygons to draw in the background and colors.
vector<POLY> polys;
vector<string> colors = {"red", "green", "blue", "yellow", "purple", "FUCHSIA",
    "NAVY", "TEAL", "AQUA", "LIME", "OLIVE", "MAROON", "GRAY", "Silver",
    "#F3E5AB", "#E4AAF2", "#B3AAF2", "#AAF2B3", "#F2B3AA", "#A2635A",
    "#A3A15B", "#5B5EA3"};

// All points and their respective countries.
vector<PT> points;
vector<int> countries;

// z-coordinate of cross product of a and b.
double cross(PT a, PT b) {
  return imag(conj(a) * b);
}

// Dot-product of a and b.
double dot(PT a, PT b) {
  return real(conj(a) * b);
}

// Counter clockwise function.
int ccw(PT a, PT b, PT c) {
  b -= a; c-= a;
  if (cross(b, c) > EPS) return +1; // counter clockwise
  if (cross(b, c) < -EPS) return -1; // clockwise
  if (dot(b, c) < 0) return + 2; // c--a--b on line
  if (norm(b) < norm(c)) return -1; // a--b--c on line
  return 0;
}

// Returns a line going through a and b.
L line(PT a, PT b) {
  L res;
  res.push_back(a);
  res.push_back(b);
  return res;
}

// Returns the direction from l[0] -> l[1].
PT dir(L &l) {
  return l[1] - l[0];
}

// Returns intersection of two lines.
PT crosspoint(L& l, L& m) {
  double a = cross(dir(l), dir(m));
  double b = cross(dir(l), l[1] - m[0]);
  return m[0] + b / a * dir(m);
}

// Returns the next point on the polygon.
PT next(POLY &poly, int i) {
  return poly[(i + 1) % poly.size()];
}

// Returns the line containing all points having the same distance to a and b.
L bisector(PT a, PT b) {
  PT A = (a + b) * PT(0.5, 0);
  return {A, A + (b - a) * PT(0, 1)};
}

// Intersects convex polygon with have plane left of l[0]->l[1].
POLY convexCut(POLY &poly, L l) {
  POLY res;
  for (int i = 0; i < (int)poly.size(); i++) {
    PT a = poly[i], b = next(poly, i);
    if (ccw(l[0], l[1], a) != -1) res.push_back(a);
    if (ccw(l[0], l[1], a) * ccw(l[0], l[1], b) < 0) {
      L lab = line(a, b);
      res.push_back(crosspoint(lab, l));
    }
  }
  return res;
}

// Computes a Voronoi cell for a given point from the vector of all other
// points. Starts with an initial convex polygon that gets cut to the Voronoi
// cell. The initial polygon needs to be completely enclosing the cell.
POLY voronoiCell(POLY &poly, vector<PT> &points, PT p) {
  for (PT &q : points) {
    if (q == p) continue;
    poly = convexCut(poly, bisector(p, q));
  }
  return poly;
}

// Reads the coordinates of the vertices and country ids to compute a Voronoi
// diagram of the background.
void voronoiBackground() {
  // Read in points.
  freopen(VORONOI_FILE, "r", stdin);
  int n;
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    double xcoord, ycoord;
    int country;
    scanf("%lf %lf %d", &xcoord, &ycoord, &country);
    points.push_back(PT(xcoord, ycoord));
    countries.push_back(country);
  }

  // Compute Voronoi cells for each points.
  // Initialize with the complete rectangle.
  for (int i = 0; i < n; i++) {
    POLY cell = {PT(0, 0), PT(SVG_WIDTH, 0), PT(SVG_WIDTH, SVG_HEIGHT),
        PT(0, SVG_HEIGHT)};
    cell = voronoiCell(cell, points, points[i]);
    polys.push_back(cell);
  }
}

// Reads the graph and all coordinates.
void graphForeground() {
  // Read graph.
  freopen(GRAPH_FILE, "r", stdin);
  int n, m;
  cin >> n >> m;
  mathematicians.resize(n);
  for (int i = 0; i < n; i++) {
    getchar();
    getline(cin, mathematicians[i].name);
    cin >> mathematicians[i].descendants;
    cin >> mathematicians[i].year;
    cin >> mathematicians[i].country;
    cin >> mathematicians[i].cx >> mathematicians[i].cy;
  }
  for (int i = 0; i < m; i++) {
    int from, to;
    cin >> from >> to;
    int ctrlPts;
    cin >> ctrlPts;
    edges.push_back(vector<PT>());
    for (int j = 0; j < ctrlPts; j++) {
      double xj, yj;
      cin >> xj >> yj;
      edges[i].push_back(PT(xj, yj));
    }
  }
}

// Returns a string containing an SVG-polygon element.
string outputPolygon(POLY &poly, string color) {
  string res = "<polygon points=\"";
  for (int i = 0; i < (int)poly.size(); i++) {
    if (i) res += " ";  
    res += to_string(poly[i].x) + "," + to_string(poly[i].y);
  }
  res += "\"";
  res += " style=\"fill: " + color + "; opacity: 0.3;\"";
  res += "/>\n";
  return res;
}

// Returns an SVG string for a mathematician/vertex.
string outputMathematician(vertex &m) {
  int offset = round(10 * log2(m.descendants + 1));
  int width = 150 + offset;
  int height = 60 + offset;

  string res = "";
  res += "<ellipse fill=\"white\" stroke=\"black\" cx=\"" +
      to_string(m.cx) + "\" cy=\"" +
      to_string(m.cy) + "\" rx=\"" +
      to_string(width / 2) + "\" ry=\"" +
      to_string(height / 2) + "\"/>";
  res += "<text x=\"" + to_string(m.cx - width / 2 + 25) + "\" y=\"" +
      to_string(m.cy + 5) + "\" textLength=\"" +
      to_string(width - 50) + "\" textAdjust=\"spacingAndGlyphs\" style=\"text-align:center;\">" + m.name + "</text>\n";
  return res;
}

// Returns an SVG string for an edge.
string outputEdge(vector<PT> &edge) {
  string res = "<polyline fill=\"none\" stroke=\"black\" points=\"";
  for (int i = 0; i < (int)edge.size(); i++) {
    if (i) res += " ";
    res += to_string(edge[i].x) + "," + to_string(edge[i].y);
  }
  res += "\" stroke-width=\"2\"/>\n";
  return res;
}

// Outputs the SVG.
void output() {
  // Open SVG file.
  freopen(OUTPUT_FILE, "w", stdout);
  printf(
      "<svg xmlns=\"%s\" width=\"%d\" height=\"%d\" viewport=\"0 0 %d %d\">\n",
      "http://www.w3.org/2000/svg",
      SVG_WIDTH, SVG_HEIGHT, SVG_WIDTH, SVG_HEIGHT);

  // Print background polygons.
  for (int i = 0; i < (int)polys.size(); i++) {
    printf("  %s\n", outputPolygon(polys[i], colors[countries[i]]).c_str());
  }

  // Print edges to the foreground.
  for (auto &e : edges) {
    printf("  %s\n", outputEdge(e).c_str());
  }

  // Print mathematicians to the foreground.
  for (auto &m : mathematicians) {
    printf("  %s\n", outputMathematician(m).c_str());
  }

  // Close SVG file.
  printf("</svg>\n");
}

int main() {
  voronoiBackground();
  graphForeground();

  output();
  return 0;
}
