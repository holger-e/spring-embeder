class Sugiyama {
  constructor(embedder, graph) {
    this._embedder = embedder;
    this._graph = graph;

    // Add the vertices and edges.
    this._vertices = [];
    for (let vert of this._graph.vertices) {
      let v = new Vertex(vert.name, 0, 0)
      this._vertices.push(v);
      this._embedder.addVertex(v);
    }
    this._edges = [];
    for (let edge of this._graph.edges) {
      let e = new Edge(this._vertices[edge.from], this._vertices[edge.to]);
      this._edges.push(e);
      this._embedder.addEdge(e);
    }
  }

  draw() {
    this._embedder.draw();
  }
}
