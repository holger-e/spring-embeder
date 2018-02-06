class Edge {
  constructor(from, to) {
    this._from = from;
    this._to = to;
  }

  get from() {
    return this._from;
  }

  get to() {
    return this._to;
  }

  draw(ctx) {
    let from = this._from.getDimensions(ctx);
    let to = this._to.getDimensions(ctx);

    let fromX = this._from.x + Math.round(from.width / 2);
    let fromY = this._from.y + Math.round(from.height / 2);
    let toX = this._to.x + Math.round(to.width / 2);
    let toY = this._to.y + Math.round(to.height / 2);

    ctx.beginPath();
    ctx.moveTo(fromX, fromY);
    ctx.lineTo(toX, toY)
    ctx.closePath();
    ctx.stroke();
  }
}
