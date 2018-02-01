var canvas;
var ctx;
var w;
var h;
var offset = 0.05;
var MAX_INT = 100000000;

function setUp(){
    canvas = document.getElementById("pane");
    ctx = canvas.getContext("2d");
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;
    w = canvas.width;
    h = canvas.height;
    
    parseInput();
    
    rand_input();
    
    draw();
    //spring_embeder();
}

var vertices = [];// = [0,2,4,5,7,7];
var positions = [];// = [new Victor(0,0),new Victor(0,-100),new Victor(100,100),new Victor(100,0), new Victor(50,50)];
var edges = [];// = [1,2,3,3,0,0,1];

function spring_embeder(){
    embed();
    draw();
}

// draws the graph on the canvas
function draw(){
    
    ctx.clearRect(0,0,w,h);
    
    var max_x = -MAX_INT;
    var max_y = -MAX_INT;
    var min_x = MAX_INT;
    var min_y = MAX_INT;
    
    for (var i = 0; i < n; ++i){ 
        max_x = Math.max(max_x,positions[i].x);
        max_y = Math.max(max_y,positions[i].y);
        min_x = Math.min(min_x,positions[i].x);
        min_y = Math.min(min_y,positions[i].y);
    }
    
    var steps_x =  ((1-offset)*w) / (max_x-min_x);
    var steps_y =  ((1-offset)*h) / (max_y-min_y);

    // calculate absolute positions
    for (var i = 0; i < n; ++i){
        positions[i] = new Victor(Math.abs( (positions[i].x - min_x) * steps_x + w * offset * 0.5), Math.abs( (max_y - positions[i].y) * steps_y + h * offset * 0.5 ));

        // draw vertices
        draw_circle(positions[i].x, positions[i].y, String(map_inv[i]));
    }

    for (var i = 0; i < n; ++i){  
        // draw edges
        for (var j = vertices[i]; j < vertices[i+1]; ++j) {
            //draw_line(positions[i].x, positions[i].y, positions[edges[j]].x, positions[edges[j]].y);
            draw_arrow(positions[i].clone(), positions[edges[j]].clone());
        }
    }
    
}

function draw_line(x1,y1,x2,y2){
    ctx.beginPath();
    ctx.moveTo(x1,y1);
    ctx.lineTo(x2,y2);
    ctx.strokeStyle = '#ff0000';
    ctx.stroke();
}

function draw_arrow(from, to){
    ctx.strokeStyle = '#ff0000';
    ctx.beginPath();
    ctx.moveTo(from.x, from.y);
    ctx.lineTo(to.x, to.y);
    ctx.stroke();
    
    ctx.beginPath();
    ctx.moveTo(to.x, to.y);
    
    from.subtract(to);
    from.normalize();
    from.multiply(new Victor(20,20));
    from.rotate(Math.PI / 10);
    to.add(from);
    ctx.lineTo(to.x, to.y);
    //alert(to.x + " - " + to.y + "\n" + a1.x + " - " + a1.y);
    to.subtract(from);
    ctx.moveTo(to.x, to.y);
    from.rotate( - Math.PI / 5);
    to.add(from);
    
    ctx.lineTo(to.x, to.y);
    ctx.stroke();
}

function draw_circle(x,y, vertex_name){
    ctx.beginPath();
    ctx.arc(x, y, 8, 0, 2 * Math.PI, false);
    ctx.fillStyle = 'black';
    ctx.strokeStyle = 'black';
    ctx.lineWidth = 3;
    ctx.fill();
    ctx.stroke();
    ctx.fillText(vertex_name,x-20,y+15);
}

var c_rep = 1;
var c_spring = 2;
var l = 1; // spring length
var max_iterations = 1000;
var eps = 0.05;
var change_weight = 1;
var delta_weight = new Victor(change_weight, change_weight);

// starts spring-embeder algorithm
function embed(){
    
    var iterations = 0;
    var max_variation = 0;
    do{
        var changeVec = [];
        // calculate forces of current layout
        for (var i = 0; i < n; ++i){
            var change = get_force(i);
            max_variation = Math.max(max_variation, change.lengthSq());
            
            
            // add new position
            changeVec.push(change.clone());
        }
        // set new positions / create new layout
        for (var i = 0; i < n; ++i){
            // set new position
            changeVec[i].multiply(delta_weight);
            positions[i].add(changeVec[i]);
        }
        
        iterations++;
    }while(iterations < max_iterations && max_variation > eps);
    
}

// delivers force for vertex v
function get_force(v){
    
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // requires edge list to be sorted ascending
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    var force = new Victor(0,0);
    
    var offset = vertices[v];
    var range = vertices[v+1];
    for (var u = 0; u < n; ++u){
        if (u == v) continue;
        if (offset < range && edges[offset] == u){
            // u and v are adjacent -> attractive force
            offset++;
            force.add(attr_force(positions[v].clone(), positions[u].clone()));
        }else{
            // u and v not adjacent -> repulsive force
            force.add(rep_force(positions[v].clone(), positions[u].clone()));
        }
    }
    //alert(force.x + " - " + force.y);
    return force;
}

// delivers attractive force for vertex v (between vertices v and u)
function attr_force(v, u){
    v.subtract(u);
    var x1 = c_spring * Math.log(v.length() / l);
    u.subtract(v);
    u.normalize();
    u.multiply(new Victor(x1,x1));
    return u;
}

// delivers repulsive force for vertex v (between vertices v and u)
function rep_force(v, u){
    v.subtract(u);
    var x1 = c_rep / v.lengthSq();
    v.subtract(u);
    v.normalize();
    v.multiply(new Victor(x1,x1));
    return v;
}

function rand_input(){
    for (var i = 0; i < n; ++i){
        positions.push(new Victor (Math.random() * w, Math.random() * h));
    }
}



var map = {};
var map_inv = [];

function parseInput()
{
    var vertex_num = 0;
    var temp_edge_count = [];
    for (var i = 0;i < n; ++i){
        map[vertices_orig[i]] = vertex_num++;
        map_inv.push(String(vertices_orig[i]));
    }
    
    var count = 0;
    var temp_edges = [];
    var last_vert = 0;
    var vertex_num = 0;
    for (var i = 0; i < m; ++i){
        var e = edges_orig[i].split(" --> ");
        if (map[e[0]] != last_vert){
            while (map[e[0]] - last_vert > 1){
                vertices.push(count);
                last_vert++;
            }
            last_vert = map[e[0]];
            temp_edges.sort();
            for (let x of temp_edges){
                edges.push(x);
            }
            temp_edges = [];
            vertices.push(count);
        }
        count++;
        temp_edges.push(map[e[1]]);
    }
    
    // finish vertex array
    vertices.push(m);
    
    console.log(vertices);
    console.log(edges);
    
}
