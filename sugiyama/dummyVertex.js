const DUMMY_DIMENSION = 10;

class DummyVertex extends Vertex {
  constructor(x, y) {
    super(String(Math.random()), x, y);

    // Gets a small width and a height, so that it can be dragged as well. 
    this._dim = {
      width: DUMMY_DIMENSION,
      height: DUMMY_DIMENSION,
      lines: []
    };
  }

  draw(ctx) {
    // Nothing needs to be drawn.
    ctx.fillStyle = "black";
    ctx.fillRect(this._x + 2, this._y + 2, 5, 5);
  }
}
