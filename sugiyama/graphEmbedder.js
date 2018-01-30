class GraphEmbedder {
  constructor(ctx) {
    // The context to draw in.
    this._ctx = ctx;

    // The graph to draw. Represented as vertex and edge list.
    this._vertices = [];
    this._edges = [];

    // The drag and drop target.
    this._dragDropTarget = undefined;

    // Event handlers for drag and drop of the vertices.
    this._ctx.canvas.addEventListener("mousedown", this._mouseDownHandler.bind(this));
    this._ctx.canvas.addEventListener("mousemove", this._mouseMoveHandler.bind(this));
    this._ctx.canvas.addEventListener("mouseup", this._mouseUpHandler.bind(this));
    this._ctx.canvas.addEventListener("mouseleave", this._mouseLeaveHandler.bind(this));
  }

  _mouseDownHandler(e) {
    let coords = this._getRelativeCoordinates(e);
    this._selectTarget(coords.x, coords.y);
  }

  _mouseMoveHandler(e) {
    let coords = this._getRelativeCoordinates(e);
    this._moveTarget(coords.x, coords.y);
  }

  _mouseUpHandler(e) {
    let coords = this._getRelativeCoordinates(e);
    this._dropTarget(coords.x, coords.y);
  }

  _mouseLeaveHandler(e) {
    let coords = this._getRelativeCoordinates(e);
    this._dropTarget(coords.x, coords.y);
  }

  _getRelativeCoordinates(e) {
    let x = e.clientX;
    let y = e.clientY;
    let bbox = this._ctx.canvas.getBoundingClientRect();
    return {
      x: x - bbox.x,
      y: y - bbox.y
    }
  }

  _dropTarget(x, y) {
    if (this._dragDropTarget === undefined) return;

    this._positionTarget(x, y);
    this._dragDropTarget = undefined;
  }

  _positionTarget(x, y) {
    let maxx = this._ctx.canvas.width - this._dragDropTarget.width;
    let maxy = this._ctx.canvas.height - this._dragDropTarget.height;
    this._dragDropTarget.x = Math.max(0, Math.min(x, maxx));
    this._dragDropTarget.y = Math.max(0, Math.min(y, maxy));
  }

  _setTarget(x, y, target) {
    if (this._dragDropTarget !== undefined) {
      this._dropTarget(x, y);
    }
    this._dragDropTarget = target;
  }

  _selectTarget(x, y) {
    // Finds the vertex that we clicked at.
    let target = this._vertices.find(v => {
      return x >= v.x && x <= v.x + v.width &&
             y >= v.y && y <= v.y + v.height;
    });
    if (target !== undefined) {
      console.log("Select", target);
      this._setTarget(x, y, target);
    }
  }

  _moveTarget(x, y) {
    if (this._dragDropTarget === undefined) return;
    this._positionTarget(x, y);
  }

  addVertex(vertex) {
    this._vertices.push(vertex);
  }

  addEdge(edge) {
    this._edges.push(edge);
  }

  draw() {
    // Clear canvas.
    this._ctx.clearRect(0, 0, this._ctx.canvas.width, this._ctx.canvas.height);

    // Draw edges.
    for (let e of this._edges) {
      e.draw(this._ctx);
    }

    // Draw vertices.
    for (let v of this._vertices) {
      v.draw(this._ctx);
    }
  }
}
