const YEAR_MIN = 1300;
const PIXELS_PER_YEAR = 20;
const X_DIFFERENCE = 200;
const OFFSET = 10;

class Sugiyama {
  constructor(embedder, graph) {
    this._embedder = embedder;
    this._graph = graph;
    this._vertices = [];
    this._edges = [];

    this._levelAssignment();
    this._addNormalVertices();
    this._addDummyVertices();
    this._countDegrees();
    this._assignXCoordinates();
    this._crossingMinimization();
    this._assignXCoordinates();
  }

  _sortByXCoordinate() {
    this._vertices.sort((a, b) => {
      if (a.x < b.x) return -1;
      if (a.x > b.x) return +1;
      return 0;
    });
  }

  _addNormalVertices() {
    for (let vert of this._graph.vertices) {
      let v = new Vertex(vert.name, 0, vert.y);
      this._vertices.push(v);
      this._embedder.addVertex(v);
    }
  }

  _yearToYCoordinate(year) {
    return (year - YEAR_MIN) * PIXELS_PER_YEAR;
  }

  _levelAssignment() {
    for (let vert of this._graph.vertices) {
      vert.y = this._yearToYCoordinate(vert.year);
    }
  }

  _addDummyVertices() {
    for (let edge of this._graph.edges) {
      let yearFrom = this._graph.vertices[edge.from].year;
      let yearTo = this._graph.vertices[edge.to].year;

      let fromIdx = edge.from;
      let toIdx = -1;
      for (let year = yearFrom + 1; year < yearTo; year++) {
        let dv = new DummyVertex(0, this._yearToYCoordinate(year));
        this._vertices.push(dv);
        this._embedder.addVertex(dv);

        toIdx = this._vertices.length - 1;
        let de = new Edge(this._vertices[fromIdx], this._vertices[toIdx]);
        this._edges.push(de);
        this._embedder.addEdge(de);

        fromIdx = toIdx;
      }
      let de = new Edge(this._vertices[fromIdx], this._vertices[edge.to]);
      this._edges.push(de);
      this._embedder.addEdge(de);
    }
  }

  _countDegrees() {
    for (let edge of this._edges) {
      let up = edge.from;
      let down = edge.to;

      if (typeof(up.below) === "undefined") {
        up.below = [down];
      } else {
        up.below.push(down);
      }

      if (typeof(down.above) === "undefined") {
        down.above = [up];
      } else {
        down.above.push(up);
      }

      if (typeof(up.downDeg) === "undefined") {
        up.downDeg = 1;
      } else {
        up.downDeg++;
      }

      if (typeof(down.upDeg) === "undefined") {
        down.upDeg = 1;
      } else {
        down.upDeg++;
      }
    }

    for (let vert of this._vertices) {
      if (typeof(vert.below) === "undefined") vert.below = [];
      if (typeof(vert.above) === "undefined") vert.above = [];
      if (typeof(vert.upDeg) === "undefined") vert.upDeg = 0;
      if (typeof(vert.downDeg) === "undefined") vert.downDeg = 0;
    }
  }

  _assignXCoordinates() {
    this._sortByXCoordinate();

    let vertsPerLevel = new Map();
    for (let vert of this._vertices) {
      if (vertsPerLevel.has(vert.y)) {
        vert.x = vertsPerLevel.get(vert.y) * X_DIFFERENCE;
        vertsPerLevel.set(vert.y, vertsPerLevel.get(vert.y) + 1);
      } else {
        vert.x = 0;
        vertsPerLevel.set(vert.y, 1);
      }
    }
  }

  _crossingMinimization() {
    // Partition vertices by level.
    let vertsPerLevel = new Map();
    for (let vert of this._vertices) {
      if (vertsPerLevel.has(vert.y)) {
        vertsPerLevel.get(vert.y).push(vert);
      } else {
        vertsPerLevel.set(vert.y, [vert]);
      }
    }

    // Map is not ordered. Stores levels in array to sort them then.
    // Assumes that no level is empty.
    let levels = [];
    for (let level of vertsPerLevel) {
      levels.push(level[1]);
    }
    levels.sort((a, b) => {
      if (a[0].y < b[0].y) return -1;
      if (a[0].y > b[0].y) return 1;
      return 0;
    });

    this._sortByXCoordinate();

    // Go down levels and rearrange with barycenter heuristic.
    let xPerLevel = new Map();
    for (let i = 1; i < levels.length; i++) {
      console.log("Level", i);
      let level = levels[i];
      for (let vert of level) {
        for (let from of vert.above) {
          if (typeof(vert.sum) === "undefined") {
            vert.sum = from.x;
          } else {
            vert.sum += from.x;
          }
        }
      }
      for (let vert of level) {
        let newX = Math.round(vert.sum / vert.upDeg);

        if (xPerLevel.has(vert.y)) {
          while (xPerLevel.get(vert.y).has(newX)) {
            newX += OFFSET;
          }
          xPerLevel.get(vert.y).add(newX);
        } else {
          let setWithX = new Set();
          setWithX.add(newX);
          xPerLevel.set(vert.y, setWithX);
        }
        vert.x = newX;
      }
    }
  }

  draw() {
    this._embedder.draw();
  }
}
