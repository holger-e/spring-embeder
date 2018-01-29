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
    
    readInputFile("test.txt");
    
    rand_input();
    
    draw();
    //spring_embeder();
}

var c_rep = 1;
var c_spring = 2;
var l = 1; // spring length
var max_iterations = 1;
var eps = 0.05;
var change_weight = 1;

var n = 5;
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
        draw_circle(positions[i].x, positions[i].y);
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

function draw_circle(x,y){
    ctx.beginPath();
    ctx.arc(x, y, 8, 0, 2 * Math.PI, false);
    ctx.fillStyle = 'black';
    ctx.strokeStyle = 'black';
    ctx.lineWidth = 3;
    ctx.fill();
    ctx.stroke();
}

// starts spring-embeder algorithm
function embed(){
    
    var iterations = 0;
    var max_variation = 0;
    do{
        for (var i = 0; i < n; ++i){
            var change = get_force(i);
            max_variation = Math.max(max_variation, change.lengthSq());
            
            change.multiply(new Victor(change_weight, change_weight));
            // set new position
            positions[i].add(change);
        }
        
        iterations++;
    }while(iterations < max_iterations && max_variation > eps);
    
}

// delivers force for vertex v
function get_force(v){
    
    // requires edge list to be sorted ascending
    
    var force = new Victor(0,0);
    
    var offset = vertices[v];
    var range = vertices[v+1];
    for (var u = 0; u < n; ++u){
        if (u == v) continue;
        if (offset < range && edges[offset] == u){
            // u and v are adjacent -> attractive force
            offset++;
            force.add(attr_force(positions[v], positions[u]));
        }else{
            // u and v not adjacent -> repulsive force
            force.add(rep_force(positions[v], positions[u]));
        }
    }
    //alert(force.x + " - " + force.y);
    return force;
}

// delivers attractive force for vertex v (between vertices v and u)
function attr_force(v, u){
    var x1 = c_spring * Math.log(v.subtract(u).length() / l);
    return u.subtract(v).normalize().multiply(new Victor(x1,x1));
}

// delivers repulsive force for vertex v (between vertices v and u)
function rep_force(v, u){
    var x1 = c_rep / v.subtract(u).lengthSq();
    return v.subtract(u).normalize().multiply(new Victor(x1,x1));
}

function rand_input(){
    for (var i = 0; i < n; ++i){
        positions.push(new Victor (Math.random() * w, Math.random() * h));
    }
}




function readInputFile(file)
{
    var rawFile = new XMLHttpRequest();
    rawFile.open("GET", file, false);
    rawFile.onreadystatechange = function ()
    {
        if(rawFile.readyState === 4)
        {
            if(rawFile.status === 200 || rawFile.status == 0)
            {
                var input = rawFile.responseText;
                var lines = input.split("\n");
                n = parseInt(lines[0]);
                var m = parseInt(lines[1]);
                var j = -1;
                for (i=2; i < m + 2; ++i){
                    var edge = lines[i].split(" ");
                    while(parseInt(edge[0]) != j){
                        j++;
                        vertices.push(i-2);
                    }
                    edges.push(parseInt(edge[1]));
                    //alert(lines[i]);
                }
                vertices.push(m);
            }
        }
    }
    rawFile.send(null);
}