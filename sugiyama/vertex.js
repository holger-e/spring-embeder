class Vertex {
  constructor(name, x, y, width, height) {
    this._name = name;
    this._width = width;
    this._height = height;
    this._x = x;
    this._y = y;
  }

  get x() {
    return this._x;
  }

  set x(x) {
    this._x = x;
  }

  get y() {
    return this._y;
  }

  set y(y) {
    this._y = y;
  }

  get width() {
    return this._width;
  }

  get height() {
    return this._height;
  }

  get center() {
    return {
      x: Math.round(this._x + this._width / 2),
      y: Math.round(this._y + this._height / 2)
    }
  }

  draw(ctx) {
    ctx.fillRect(this._x, this._y, this._width, this._height);
  }
}
