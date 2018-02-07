const YEAR_MIN = 1300;
const PIXELS_PER_YEAR = 20;
const X_DIFFERENCE = 200;
const OFFSET = 10;

const MATHEMATICIAN = 0;
const DUMMY = 1;

class Test {
  constructor(embedder, graph) {
    this._embedder = embedder;
    this._graph = graph;
    this._vertices = [];

    for (let vert of this._graph.vertices) {
      let v;
      if (vert.type == MATHEMATICIAN) {
        v = new Vertex(vert.name, vert.x, vert.y);
      } else if (vert.type == DUMMY) {
        v = new DummyVertex(vert.x, vert.y);
      }
      this._embedder.addVertex(v);
      this._vertices.push(v);
    }
    for (let edge of this._graph.edges) {
      let e = new Edge(this._vertices[edge.from],
                       this._vertices[edge.to]);
      this._embedder.addEdge(e);
    }
  }

  draw() {
    this._embedder.draw();
  }
}
