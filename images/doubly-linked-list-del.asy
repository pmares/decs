size(300,0);

void e(real offset) {
	draw(shift(offset-1.5,0)*unitsquare);
	draw(shift(offset-1.5,0)*unitsquare);
	draw(shift(offset-0.5)*unitsquare);
	draw(shift(offset+0.5,0)*unitsquare);
	dot((offset-7/6,1/3), defaultpen+linewidth(2));
	dot((offset+7/6,2/3), defaultpen+linewidth(2));
}

path link(real start, real end, bool rightP=true, bool skip=false) {
	if (!skip && rightP) {
		return (start+7/6,2/3)--(end-3/2,2/3);
	} else if (!skip && !rightP) {
		return (start-7/6,1/3)--(end+3/2,1/3);
	} else if (skip && rightP) {
		return (start+7/6,2/3){right}..(start+17/6,2/3){right}..(start+19/6,1){up}..(start+21/6,4/3){right}..(end-21/6,4/3){right}..(end-19/6,1){down}..(end-17/6,2/3){right}..(end-3/2,2/3){right};
	} else {
		return (start-7/6,1/3){left}..(start-17/6,1/3){left}..(start-19/6,0){down}..(start-21/6,-1/3){left}..(end+21/6,-1/3){left}..(end+19/6,0){up}..(end+17/6,1/3){left}..(end+3/2,1/3){left};
	}
}

e(-5);
e(0);
e(5);

draw(link(-5,5,true,true), Arrow);
draw(link(5,-5,false,true), Arrow);
draw(link(0,-5,false), Arrow);
draw(link(0,5), Arrow);


label("$x$",(0,-1/3),S);
