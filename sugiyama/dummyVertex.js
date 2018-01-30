const DUMMY_DIMENSION = 10;

class DummyVertex extends Vertex {
  constructor(x, y) {
    super("", x, y);
    // Gets a small width and a height, so that it can be dragged as well. 
    this._dim = {
      width: DUMMY_DIMENSION,
      height: DUMMY_DIMENSION,
      lines: []
    };
  }

  draw(ctx) {
    // Nothing needs to be drawn.
  }
}
