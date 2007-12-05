size(250,0);

real dotsize = 2;
real arrowsize = 6;
real bmaxx = 11;
real bminx = 0;
real bmaxy = 16;
real bminy = 0;

path head = (0,0)--(1,0)--(1,2)--(0,2)--cycle;

void node(pair offset) {
	draw(shift(offset)*unitsquare);
	dot((offset.x+1/4,offset.y+1/4), defaultpen+linewidth(dotsize));
	dot((offset.x+3/4,offset.y+1/4), defaultpen+linewidth(dotsize));
	dot((offset.x+1/4,offset.y+3/4), defaultpen+linewidth(dotsize));
	dot((offset.x+3/4,offset.y+3/4), defaultpen+linewidth(dotsize));
}

void header(pair offset, string name = "", string size = "") {
	draw(shift(offset)*head);
	dot((offset.x+1/4,offset.y+1/4), defaultpen+linewidth(dotsize));
	dot((offset.x+3/4,offset.y+1/4), defaultpen+linewidth(dotsize));
	dot((offset.x+1/4,offset.y+7/4), defaultpen+linewidth(dotsize));
	dot((offset.x+3/4,offset.y+7/4), defaultpen+linewidth(dotsize));
	if (name != "") label(name,(offset.x+1/2,offset.y+1),N);
	if (size != "") label(size,(offset.x+1/2,offset.y+1),S);
}

// Convert from real coordinates to matrix coordinates
pair rtmc(pair coords) {
	return ((coords.x-1)/2, (13-coords.y)/2);
}
pair rtmc(real x, real y) {
	return rtmc((x, y));
}

// Convert from matrix coordinates to real coordinates
pair mtrc(pair coords) {
	return (2*coords.x+1, 13-2*coords.y);
}

pair mtrc(real x, real y) {
	return mtrc((x, y));
}


path link(pair start, pair end, bool skip=false) {
	real sx = start.x;
	real sy = start.y;
	real ex = end.x;
	real ey = end.y;
	
	// Don't allow zero length lines and lines which are not horizontal or vertical.
	if (sx != ex && sy != ey) return (0,0);
	if (sx == ex && sy == ey) return (0,0);
	
	if (sy == ey) {  // Horizontal line.
		if (sx < ex) {  // Pointing right.
			if (sx+3/4 < bminx) sx = bminx-3/4;
			return (sx+3/4, sy+3/4)--(ex, ey+3/4);
		} else {  // Pointing left.
			if (sx+1/4 > bmaxx) sx = bmaxx-1/4;
			return (sx+1/4, sy+1/4)--(ex+1, ey+1/4);
		}
	} else if (sx == ex) {  // Vertical line.
		if (sy > ey) {  // Pointing down.
			if (sy+1/4 > bmaxy) sy = bmaxy-1/4;
			return (sx+3/4, sy+1/4)--(ex+3/4, ey+1);
		} else {  // Pointing up.
			if (sy+3/4 < bminy) sy = bminy-3/4;
			return (sx+1/4, sy+3/4)--(ex+1/4, ey);
		}
	}
	return (0,0);
	
//	} else if (skip && rightP) {
//		return (start+7/6,2/3){right}..(start+17/6,2/3){right}..(start+19/6,1){up}..(start+21/6,4/3){right}..(end-21/6,4/3){right}..(end-19/6,1){down}..(end-17/6,2/3){right}..(end-3/2,2/3){right};
//	} else {
//		return (start-7/6,1/3){left}..(start-17/6,1/3){left}..(start-19/6,0){down}..(start-21/6,-1/3){left}..(end+21/6,-1/3){left}..(end+19/6,0){up}..(end+17/6,1/3){left}..(end+3/2,1/3){left};
//	}
}

void drawlink(pair start, pair end, bool arrowhead=true, bool skip=false) {
	path g = link(start, end, skip);
	if (arrowhead)
		draw(g, Arrow(arrowsize)); 
	else
		draw(g); 
}


// Column headers.
label("$h$",(1,13+1),W);
header(mtrc(0,0));
header(mtrc(1,0), "A", (string)2);
header(mtrc(2,0), "B", (string)2);
header(mtrc(3,0), "C", (string)3);
header(mtrc(4,0), "D", (string)2);


// Horizontal column header links.
drawlink(mtrc(-1,-0.5), mtrc(0,-0.5));
drawlink(mtrc(0,-0.5), mtrc(1,-0.5));
drawlink(mtrc(1,-0.5), mtrc(2,-0.5));
drawlink(mtrc(2,-0.5), mtrc(3,-0.5));
drawlink(mtrc(3,-0.5), mtrc(4,-0.5));
drawlink(mtrc(4,-0.5), mtrc(5,-0.5),false);

drawlink(mtrc(5,0), mtrc(4,0));
drawlink(mtrc(4,0), mtrc(3,0));
drawlink(mtrc(3,0), mtrc(2,0));
drawlink(mtrc(2,0), mtrc(1,0));
drawlink(mtrc(1,0), mtrc(0,0));
drawlink(mtrc(0,0), mtrc(-1,0),false);


// Nodes
node(mtrc(1,1));
node(mtrc(2,2));
node(mtrc(3,2));
node(mtrc(1,3));
node(mtrc(4,3));
node(mtrc(3,4));
node(mtrc(4,4));
node(mtrc(2,5));
node(mtrc(3,6));

// Horizontal node links.
drawlink(mtrc(-1,1), mtrc(1,1));
drawlink(mtrc(1,1), mtrc(5,1),false);
drawlink(mtrc(-1,2), mtrc(2,2));
drawlink(mtrc(2,2), mtrc(3,2));
drawlink(mtrc(3,2), mtrc(5,2),false);
drawlink(mtrc(-1,3), mtrc(1,3));
drawlink(mtrc(1,3), mtrc(4,3));
drawlink(mtrc(4,3), mtrc(5,3),false);
drawlink(mtrc(-1,4), mtrc(3,4));
drawlink(mtrc(3,4), mtrc(4,4));
drawlink(mtrc(4,4), mtrc(5,4),false);
drawlink(mtrc(-1,5), mtrc(2,5));
drawlink(mtrc(2,5), mtrc(5,5),false);
drawlink(mtrc(-1,5), mtrc(2,5));
drawlink(mtrc(2,5), mtrc(5,5),false);
drawlink(mtrc(-1,6), mtrc(3,6));
drawlink(mtrc(3,6), mtrc(5,6),false);

drawlink(mtrc(1,1), mtrc(-1,1), false);
drawlink(mtrc(5,1), mtrc(1,1));
drawlink(mtrc(2,2), mtrc(-1,2), false);
drawlink(mtrc(3,2), mtrc(2,2));
drawlink(mtrc(5,2), mtrc(3,2));
drawlink(mtrc(1,3), mtrc(-1,3), false);
drawlink(mtrc(4,3), mtrc(1,3));
drawlink(mtrc(5,3), mtrc(4,3));
drawlink(mtrc(3,4), mtrc(-1,4), false);
drawlink(mtrc(4,4), mtrc(3,4));
drawlink(mtrc(5,4), mtrc(4,4));
drawlink(mtrc(2,5), mtrc(-1,5), false);
drawlink(mtrc(5,5), mtrc(2,5));
drawlink(mtrc(2,5), mtrc(-1,5), false);
drawlink(mtrc(5,5), mtrc(2,5));
drawlink(mtrc(3,6), mtrc(-1,6), false);
drawlink(mtrc(5,6), mtrc(3,6));


// Vertical column header links.
drawlink(mtrc(1,-1.5), mtrc(1,-0.5));
drawlink(mtrc(2,-1.5), mtrc(2,-0.5));
drawlink(mtrc(3,-1.5), mtrc(3,-0.5));
drawlink(mtrc(4,-1.5), mtrc(4,-0.5));
drawlink(mtrc(1,-0.5), mtrc(1,-1.5), false);
drawlink(mtrc(2,-0.5), mtrc(2,-1.5), false);
drawlink(mtrc(3,-0.5), mtrc(3,-1.5), false);
drawlink(mtrc(4,-0.5), mtrc(4,-1.5), false);

// Vertical node links.
drawlink(mtrc(1,0), mtrc(1,1));
drawlink(mtrc(1,1), mtrc(1,3));
drawlink(mtrc(1,3), mtrc(1,7), false);
drawlink(mtrc(2,0), mtrc(2,2));
drawlink(mtrc(2,2), mtrc(2,5));
drawlink(mtrc(2,5), mtrc(2,7), false);
drawlink(mtrc(3,0), mtrc(3,2));
drawlink(mtrc(3,2), mtrc(3,4));
drawlink(mtrc(3,4), mtrc(3,6));
drawlink(mtrc(3,6), mtrc(3,7), false);
drawlink(mtrc(4,0), mtrc(4,3));
drawlink(mtrc(4,3), mtrc(4,4));
drawlink(mtrc(4,4), mtrc(4,7), false);

drawlink(mtrc(1,1), mtrc(1,0));
drawlink(mtrc(1,3), mtrc(1,1));
drawlink(mtrc(1,7), mtrc(1,3));
drawlink(mtrc(2,2), mtrc(2,0));
drawlink(mtrc(2,5), mtrc(2,2));
drawlink(mtrc(2,7), mtrc(2,5));
drawlink(mtrc(3,2), mtrc(3,0));
drawlink(mtrc(3,4), mtrc(3,2));
drawlink(mtrc(3,6), mtrc(3,4));
drawlink(mtrc(3,7), mtrc(3,6));
drawlink(mtrc(4,3), mtrc(4,0));
drawlink(mtrc(4,4), mtrc(4,3));
drawlink(mtrc(4,7), mtrc(4,4));

real margin = 0.02;
draw((bminx-margin, bminy-margin)--(bmaxx+margin, bminy-margin)--(bmaxx+margin, bmaxy+margin)--(bminx-margin, bmaxy+margin)--cycle, dotted);