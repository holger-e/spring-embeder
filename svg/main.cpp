#include <bits/stdc++.h>

// Shortcuts to get coordinates of point represented as a complex number.
#define x real()
#define y imag()

// Dimensions of the SVG.
#define SVG_WIDTH 66500
#define SVG_HEIGHT 73000

// Path to the file containing the complete foreground drawing information.
#define GRAPH_TEST_FILE "test_everything.txt"
#define GRAPH_FILE "../layering_bigGraph/bigGraph_complete_layout.txt"

// Name of the SVG that the drawing is exported in.
#define OUTPUT_FILE "graph.svg"

// Epsilon value for counter clockwise function.
#define EPS 1e-6

// The number of characters in the name before a line break.
#define CHARS_PER_LINE 19

// Width and height of a vertex.
#define VERT_WIDTH 120
#define VERT_HEIGHT 60

// How much space is between two year annotations.
#define YEAR_ANNOTATION_OFFSET 5000

// Dimensions of the legend.
#define LEGEND_X 0
#define LEGEND_Y 0
#define LEGEND_WIDTH 250
#define LEGEND_HEIGHT 1200

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
map<string, string> colors;

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
  // Extract array with all points.
  vector<PT> points;
  for (auto &m : mathematicians) {
    points.push_back(PT (m.cx, m.cy));
  }

  // Compute Voronoi cells for each points.
  // Initialize with the complete rectangle.
  for (auto &m : mathematicians) {
    POLY cell = {PT(0, 0), PT(SVG_WIDTH, 0), PT(SVG_WIDTH, SVG_HEIGHT),
        PT(0, SVG_HEIGHT)};
    cell = voronoiCell(cell, points, PT(m.cx, m.cy));
    polys.push_back(cell);
  }
}

// Reads the graph and all coordinates.
void readGraph() {
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

// Outputs a single row of the name. If the name is long, spacing is reduced.
string outputNameRow(double cx, double dy, string row) {
  string res = "";
  if (row.length() >= CHARS_PER_LINE - 2) {
    res += "<tspan x=\"" + to_string(cx) + "\" dy=\"" +
           to_string(dy) + "\" textLength=\"" + to_string(VERT_WIDTH - 10) +
           "\" textAdjust=\"spacingAndGlyphs\">" + row + "</tspan>";
  } else {
    res += "<tspan x=\"" + to_string(cx) + "\" dy=\"" +
           to_string(dy) + "\">" + row + "</tspan>";
  }
  return res;
}

// Outputs the name. Breaks long names in two or three rows.
string outputName(double cx, double cy, string name) {
  string res = "";

  if (name.length() >= CHARS_PER_LINE) {
    int breakSpace = name.find_last_of(" ", CHARS_PER_LINE);
    string line1 = name.substr(0, breakSpace);
    string line2 = name.substr(breakSpace + 1);
    res += "<text y=\"" + to_string(cy) + "\" text-anchor=\"middle\">";

    if (line2.length() >= CHARS_PER_LINE) {
      breakSpace = line2.find_last_of(" ", CHARS_PER_LINE);
      string line21 = line2.substr(0, breakSpace);
      string line22 = line2.substr(breakSpace + 1);
      res += outputNameRow(cx, -11, line1);
      res += outputNameRow(cx, 16, line21);
      res += outputNameRow(cx, 16, line22);
    } else {
      res += outputNameRow(cx, -3, line1);
      res += outputNameRow(cx, 16, line2);
    }
    res += "</text>";
  } else {
    if (name.length() >= CHARS_PER_LINE - 2) {
      res += "<text x=\"" + to_string(cx) + "\" y=\"" + to_string(cy + 4) +
           "\" text-anchor=\"middle\" textLength=\"" +
           to_string(VERT_WIDTH - 10) + "\" textAdjust=\"spacingAndGlyphs\">" +
           name + "</text>\n";
    } else {
      res += "<text x=\"" + to_string(cx) + "\" y=\"" + to_string(cy + 4) +
           "\" text-anchor=\"middle\">" + name + "</text>\n";
    }
  }
  return res;
}

// Returns an SVG string for a mathematician/vertex.
string outputMathematician(vertex &m) {
  int width = VERT_WIDTH;
  int height = VERT_HEIGHT;

  string res = "";
  res += "<rect fill=\"white\" stroke=\"black\" x=\"" +
      to_string(m.cx - width / 2) + "\" y=\"" +
      to_string(m.cy - height / 2) + "\" width=\"" +
      to_string(width) + "\" height=\"" +
      to_string(height) + "\" rx=\"7\" ry=\"7\"/>";
  res += outputName(m.cx, m.cy, m.name);
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

// Returns an SVG dotted line for a year.
string outputYearLine(int ycoord, int year) {
  string res = "";
  res += "<polyline fill=\"none\" stroke=\"lightgray\" points=\"0," +
          to_string(ycoord) + " " + to_string(SVG_WIDTH) +
          "," + to_string(ycoord) +
          "\" stroke-width=\"1\" stroke-dasharray=\"5, 10\"/>";
  for (int i = YEAR_ANNOTATION_OFFSET; i < SVG_WIDTH; i += YEAR_ANNOTATION_OFFSET) {
    res += "<text style=\"fill: gray;\" x=\"" + to_string(i) +
           "\" y=\"" + to_string(ycoord - 10) + "\">" + to_string(year) + "</text>";
  }
  return res;
}

// Outputs the legend.
string outputLegend() {
  string res = "";
  res += "<rect x=\"" + to_string(LEGEND_X) + "\" y=\"" + to_string(LEGEND_Y) +
         "\" width=\"" + to_string(LEGEND_WIDTH) + "\" height=\"" +
         to_string(LEGEND_HEIGHT) + "\" style=\"fill: white;\"/>";
  int cx = LEGEND_X + 50;
  int cy = LEGEND_Y + 30;
  res += "<text x=\"" + to_string(cx) + "\" y=\"" + to_string(cy) +
         "\">Colors of Countries:</text>";
  cy += 20;
  for (auto &c : colors) {
    res += "<rect x=\"" + to_string(cx) + "\" y=\"" + to_string(cy) +
           "\" width=\"20\" height=\"20\" style=\"fill: " +
           c.second + ";\" stroke=\"black\"/>";
    res += "<text x=\"" + to_string(cx + 40) + "\" y=\"" +
           to_string(cy + 17) + "\">" + c.first + "</text>";
    cy += 50;
  }
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
  printf("<rect x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" style=\"fill: white;\"/>\n", SVG_WIDTH, SVG_HEIGHT);
  for (int i = 0; i < (int)polys.size(); i++) {
    printf("  %s\n", outputPolygon(polys[i], colors[mathematicians[i].country]).c_str());
  }

  // Print year lines.
  set<pair<int, int>> years;
  for (auto &m : mathematicians) {
    years.insert(pair<int, int> (m.cy, m.year));
  }
  for (auto &s : years) {
    printf("  %s\n", outputYearLine(s.first, s.second).c_str());
  }

  // Print legend.
  printf("  %s\n", outputLegend().c_str());

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
  colors["Austria"] = "#876FFF";
  colors["Belgium"] = "#0069FF";
  colors["Brazil"] = "#FF003F";
  colors["CzechRepublic"] = "#6FFFC2";
  colors["Denmark"] = "#8DF96B";
  colors["Estonia"] = "#F9766B";
  colors["Finland"] = "#2D953C";
  colors["France"] = "#F986EB";
  colors["Germany"] = "#84ACFF";
  colors["Greece"] = "#6BF9EA";
  colors["Italy"] = "#FFC46F";
  colors["Netherlands"] = "#FF7E7C";
  colors["Norway"] = "#5D2D95";
  colors["Poland"] = "#952D7F";
  colors["Russia"] = "#FF00F1";
  colors["Slovenia"] = "#95912D";
  colors["Spain"] = "#2D958F";
  colors["Sweden"] = "#2D3595";
  colors["Switzerland"] = "#FFFFAF";
  colors["Ukraine"] = "#39952D";
  colors["UnitedKingdom"] = "#D30000";
  colors["UnitedStates"] = "#A1E6AB";

  readGraph();
  voronoiBackground();
  output();
  return 0;
}
