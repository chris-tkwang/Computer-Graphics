
#include "WorkingScene.h"

// This file includes the basic functions that your program must fill in.  
// Your assignment consists of filling in parts that say /* YOUR CODE HERE */
// There are many helpful functions that can be found in Curve.cpp! In particular,
// take a look at moveActivePoint, drawLine, connectTheDots, etc.

// What happens when you drag the mouse to x and y?  
// In essence, you are dragging control points on the curve.
void WorkingScene::drag(int x, int y) {
	/* YOUR CODE HERE */
	//you must figure out how to transform x and y so they make sense
	//update oldx, and oldy
	//make sure scene gets redrawn
                
	theOnlyCurve->moveActivePoint((x-oldx) / (float)width, (oldy-y) / (float)height);
	oldx = x;
	oldy = y;
	glutPostRedisplay();
}

// Mouse motion.  You need to respond to left clicks (to add points on curve) 
// and right clicks (to delete points on curve) 
void WorkingScene::mouse(int button, int state, int x, int y) {
	if (theOnlyCurve && state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			/* YOUR CODE HERE */
			theOnlyCurve->addPoint(x / (float)width, (height-y) / (float)height);

		}
		if (button == GLUT_RIGHT_BUTTON) {	
			/* YOUR CODE HERE */
			theOnlyCurve->updateActivePoint(x / (float)width, (height-y) / (float)height);
			theOnlyCurve->deleteActivePoint();
			
		}
	}

	/* YOUR CODE HERE */
	//update oldx, and oldy
	//make sure scene gets redrawn
	oldx = x;
	oldy = y;
	glutPostRedisplay();
}



#include "Bezier.h"

void Bezier::draw(int levelOfDetail) {
	connectTheDots();

	if (points.size() > 1){
		Pvector fu;

		for (int i=0; i<=levelOfDetail; i++){
			float u = 1.0 / levelOfDetail * i;
			Pvector ps;
			for (int j=points.size(); j>0; j--){
				if (j == points.size()){
					for (int k=0; k<points.size(); k++){
						ps.push_back(points[k]);
					}
					continue;
				}
				
				for(int k=0; k<j; k++){
					float x, y;
					x = (1-u) * ps[k].x + u * ps[k+1].x;
					y = (1-u) * ps[k].y + u * ps[k+1].y;
					ps[k] = Point(x, y);
				}
			}
			fu.push_back(ps[0]);
		}

		for (int i=0; i<levelOfDetail; i++){
			drawLine(fu[i].x, fu[i].y, fu[i+1].x, fu[i+1].y);
		}
	}

}

#include "Bspline.h"

// The B-Spline drawing routine.  
// Remember to call drawSegment (auxiliary function) for each set of 4 points.
void Bspline::draw(int levelOfDetail) {

	connectTheDots();

	/* YOUR CODE HERE */
	if (points.size() > 3){
		Pvector::iterator it;
		for (it = points.begin(); it != points.end()-3; it++){
			drawSegment(it, it+1, it+2, it+3, levelOfDetail);
		}	
	}	
}

void Bspline::drawSegment(Pvector::iterator p1, Pvector::iterator p2, 
		Pvector::iterator p3, Pvector::iterator p4, int levelOfDetail) {

	vector<float> xs, ys;
	for (int i=0; i<=levelOfDetail; i++){
		float u = 1.0 / levelOfDetail * i;
		float c1 = -u*u*u + 3*u*u - 3*u + 1;
		float c2 = 3*u*u*u - 6*u*u + 4;
		float c3 = -3*u*u*u + 3*u*u + 3*u + 1;
		float c4 = u*u*u;

		float fx = (p1->x * c1 + p2->x * c2 + p3->x * c3 + p4->x * c4) / 6;
		float fy = (p1->y * c1 + p2->y * c2 + p3->y * c3 + p4->y * c4) / 6;
		xs.push_back(fx);
		ys.push_back(fy);
	}

	for (int i=0; i<levelOfDetail; i++){
		drawLine(xs[i], ys[i], xs[i+1], ys[i+1]);
	}

	Point p(xs[levelOfDetail], ys[levelOfDetail]);
	p.draw();
}

#include "Bezier2.h"

//This function is provided to aid you.
//It should be used in the spirit of recursion, though you may choose not to.
//This function takes an empty vector of points, accum
//It also takes a set of control points, pts, and fills accum with
//the control points that correspond to the next level of detail.
void accumulateNextLevel(Pvector* accum, Pvector pts) {
	if (pts.empty()) return; 
	accum->push_back(*(pts.begin()));
	if (pts.size() == 1) return;
	for (Pvector::iterator it = pts.begin(); it != pts.end() - 1; it++) {
		/* YOUR CODE HERE  (only one to three lines)*/
		it->x = (it->x + (it+1)->x) / 2;
		it->y = (it->y + (it+1)->y) / 2;
	}
	//save the last point
	Point last = *(pts.end()-1);
	pts.pop_back();
	//recursive call
	accumulateNextLevel(accum, pts);
	accum->push_back(last);
}


// The basic draw function for Bezier2.  Note that as opposed to Bezier, 
// this draws the curve by recursive subdivision.  So, levelofdetail 
// corresponds to how many times to recurse.  
void Bezier2::draw(int levelOfDetail) {
	//This is just a trick to find out if this is the top level call
	//All recursive calls will be given a negative integer, to be flipped here
	if (levelOfDetail > 0) {
		connectTheDots();
	} else {
		levelOfDetail = -levelOfDetail;
	}

	//Base case.  No more recursive calls.
	if (levelOfDetail <= 1) {
		if (points.size() >= 2) {
			for (Pvector::iterator it = points.begin(); it != points.end() - 1; it++) {

				/* YOUR CODE HERE */
				drawLine(it->x, it->y, (it+1)->x, (it+1)->y);

			}
		}
	} else {
		if (points.size() >= 2) {
			Pvector* accum = new Pvector();
			Bezier2 left, right;

		//add the correct points to 'left' and 'right'.
		//You may or may not use accum as you see fit.
		/* YOUR CODE HERE */
			Pvector pts;
			for (int i=0; i<points.size(); i++){
				pts.push_back(points[i]);
			}
			accumulateNextLevel(accum, pts);

			for (int i=0; i <= accum->size()/2; i++){
				left.addPoint((*accum)[i].x, (*accum)[i].y);
				right.addPoint((*accum)[i+accum->size()/2].x, (*accum)[i+accum->size()/2].y);
			} 

			left.draw(1-levelOfDetail);
			right.draw(1-levelOfDetail);
			delete accum;
		}
	}
}
