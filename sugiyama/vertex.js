const MAX_WIDTH = 150;
const LINE_HEIGHT = 15;
const FONT_SIZE = 12;

class Vertex {
  constructor(name, x, y) {
    this._name = name;
    this._x = x;
    this._y = y;
    this._dim = undefined;
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
    if (typeof this._dim === "undefined") return 0;
    return this._dim.width;
  }

  get height() {
    if (typeof this._dim === "undefined") return 0;
    return this._dim.height;
  }

  getDimensions(ctx) {
    if (typeof this._dim !== "undefined") return this._dim;

    // Measure the name.
    ctx.font = FONT_SIZE + "px sans serif";
    let textMetric = ctx.measureText(this._name);

    // If it fits in one line, just use it.
    if (textMetric.width < MAX_WIDTH) {
      this._dim = {
        width: textMetric.width,
        height: LINE_HEIGHT,
        lines: [this._name]
      };
      return this._dim;
    }

    // If it doesn't fit into one line, we break the name at spaces.
    let words = this._name.split(" ");
    let curLine = "";
    let lines = [];
    // This can result in shitty drawings, if one part of the name is too long
    // for single line. In this case we should either break differently or
    // increase MAX_WIDTH.
    for (let word of words) {
      let testText = curLine.length == 0 ? word : curLine + " " + word;
      let lineMetric = ctx.measureText(testText);
      if (lineMetric.width <= MAX_WIDTH) {
        curLine = testText;
      } else {
        lines.push(curLine);
        curLine = word;
      }
    }
    if (curLine.length > 0) {
      lines.push(curLine);
    }

    let width = 0;
    let height = 0;
    for (let line of lines) {
      let lineMetric = ctx.measureText(line);
      width = Math.max(width, lineMetric.width);
      height += LINE_HEIGHT;
    }
    this._dim = {
      width: width,
      height: height,
      lines: lines
    };
    return this._dim;
  }

  draw(ctx) {
    let dim = this.getDimensions(ctx);
    ctx.clearRect(this._x, this._y, dim.width, dim.height);

    ctx.fillStyle = "black";
    for (let i = 0; i < dim.lines.length; i++) {
      let line = dim.lines[i];
      ctx.font = FONT_SIZE + "px sans serif";
      ctx.fillText(line, this._x, this._y + (i + 1) * LINE_HEIGHT - 4, dim.width);
    }
  }
}
