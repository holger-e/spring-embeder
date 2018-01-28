class Edge {
  constructor(from, to) {
    this._from = from;
    this._to = to;
  }

  draw(ctx) {
    let from = this._from.center;
    let to = this._to.center;

    ctx.beginPath();
    ctx.moveTo(from.x, from.y);
    ctx.lineTo(to.x, to.y)
    ctx.closePath();
    ctx.stroke();
  }
}
