#include <cstdlib>
#include <cstdio>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

class Vect {
	public:
		double x, y, a;
		Vect(double _x, double _y, double _a) {
			x=_x;
			y=_y;
			a=_a;
		}
		Vect *getNewNext(double step, double angleChange) {
			return new Vect(
				x + step*sin(a),
				y + step*cos(a),
				a + angleChange);
		}
};

int main(int argc, char *argv[]) {
	if(argc != 8) {
		printf("Collatz Conjecture Visualizer\n");
		printf("Usage: ./collatz <limit> <angle left> <angle right> <step>\n");
		printf(" | <limit> - the top number to visualize\n");
		printf(" | <angle left> - angle per each iteration when turning left\n");
		printf(" | <angle right> - angle per each iteration when turning right\n");
		printf(" | <step left> - step per each iteration when turning left\n");
		printf(" | <step right> - step per each iteration when turning right\n");
		printf(" | <thickness inner> - thickes of the inner line\n");
		printf(" | <thickness outer> - thickes of the whole line\n");
		return 1;
	}
	
	
	int limit = atoi(argv[1]);
	double angleLeft = atof(argv[2]);
	double angleRight = atof(argv[3]);
	double stepLeft = atof(argv[4]);
	double stepRight = atof(argv[5]);
	int thicknessInner = atoi(argv[6]);
	int thicknessOuter = atoi(argv[7]);
	printf("DEBUG: limit=%d angleLeft=%f angleRight=%f stepLeft=%f stepRight=%fi thicknessInner=%d thicknessOuter=%d\n",
		limit, angleLeft, angleRight, stepLeft, stepRight, thicknessInner, thicknessOuter);

	printf("DEBUG: Calculating Colltaz conjecture\n");
	vector<int> jumps(limit+1, 0);
	for(vector<int>::iterator i = jumps.begin(); i != jumps.end(); i++) {
		int index = distance(jumps.begin(), i);
		if(index == 0) {
			*i = -1;
			continue;
		}
		if(index == 1) {
			*i = 0;
			continue;
		}
		if(index % 2) {
			int newval = 3*index+1;
			if(newval > limit) {
				*i = -2;
				continue;
			}
			*i = newval;
			continue;
		}
		*i = index/2;
	}

	printf("DEBUG: Generating tree branches\n");
	vector<vector<int>*> paths;
	for(vector<int>::reverse_iterator i = jumps.rbegin(); i != jumps.rend(); i++) {
		int index = distance(jumps.begin(), i.base()) - 1;
		vector<int> *list = new vector<int>;
		int next = index;
		list->push_back(next);
		while(jumps[next] >= 0) {
			next = jumps[next];
			list->push_back(next);
		}
		paths.push_back(list);
	}

	printf("DEBUG: Removing detached branches\n");
	for(vector<vector<int>*>::iterator i = paths.begin(); i != paths.end(); i++) {
		if((*i)->size() < 2 || (*i)->back() != 0) {
			delete (*i);
			paths.erase(i--);
		}
	}
	
	printf("DEBUG: Removing all unnesesary subseries\n");
	/*
	for(vector<vector<int>*>::reverse_iterator i = paths.rbegin(); i != paths.rend()-1; i++) {
		for(vector<vector<int>*>::reverse_iterator j = i+1; j != paths.rend(); j++) {
			printf("(%d)(%d)\n", (int)distance(paths.begin(), i.base()), (int)distance(paths.begin(), j.base()));
			bool theSamePrefixes = true;
			for(int k = 0; k < min((*i)->size(), (*j)->size()); k++) {
				printf("[vi=%d][vj=%d]\n", (**i)[k], (**j)[k]);
				if((*i)[k] != (*j)[k]) {
					theSamePrefixes = false;
				}
			}
			printf("[%d]\n", theSamePrefixes);
			// TODO: finish
		}
	}
	*/

	printf("DEBUG: Calculating physical branches\n");
	vector<vector<Vect*>*> physicalPaths;
	for(vector<vector<int>*>::reverse_iterator i = paths.rbegin(); i != paths.rend(); i++) {
		physicalPaths.push_back(new vector<Vect*>);
		physicalPaths.back()->push_back(new Vect(0, 0, M_PI));
		for(vector<int>::reverse_iterator j = (*i)->rbegin() + 1; j != (*i)->rend() + 1; j++) {
			printf("%d ", *j);
			Vect *newVect;
			newVect = physicalPaths.back()->back()->getNewNext((*j)%2?stepLeft:stepRight, (*j)%2?angleLeft:angleRight);
			physicalPaths.back()->push_back(newVect);
		}
		printf("\n");
	}

	printf("DEBUG: Preparing ImageMagick script\n");
	int imgSize = 2200;
	FILE *fp;
	fp = fopen("generated/cmd.sh", "w");
	fprintf(fp, "#!/bin/bash\n");
	fprintf(fp, "c0=white\n");
	fprintf(fp, "c1=#45961b\n");
	fprintf(fp, "c2=#67bf39\n");
	fprintf(fp, "convert -size %dx%d xc:$c0 \\\n", imgSize, imgSize);
	for(vector<vector<Vect*>*>::iterator i = physicalPaths.begin(); i != physicalPaths.end(); i++) {
		string points;
		for(vector<Vect*>::iterator j = (*i)->begin(); j != (*i)->end(); j++) {
			points += " " + to_string(imgSize/2+(int)round((*j)->x)) + "," + to_string(imgSize/2+(int)round((*j)->y));
		}
		int lastX = imgSize/2+(int)round((*i)->back()->x);
		int lastY = imgSize/2+(int)round((*i)->back()->y);
		// TODO: check out something better than bezier curve
		fprintf(fp, "\t-fill none -stroke $c1 -strokewidth %d -draw \"stroke-linecap round bezier%s\" \\\n",
			thicknessOuter, points.c_str());
		fprintf(fp, "\t-fill $c1 -stroke none -draw \"circle %d,%d %d,%d\" \\\n",
			lastX, lastY, lastX, lastY + thicknessOuter/2);
		fprintf(fp, "\t-fill none -stroke $c2 -strokewidth %d -draw \"stroke-linecap round bezier%s\" \\\n",
			thicknessInner, points.c_str());
		fprintf(fp, "\t-fill $c2 -stroke none -draw \"circle %d,%d %d,%d\" \\\n",
			lastX, lastY, lastX, lastY + thicknessInner/2);
	}
	fprintf(fp, "\t-trim -matte -fill none -draw 'color 0,0 replace' \\\n");
	fprintf(fp, "\tgenerated/out.png");
	fclose(fp);

	//-fill none -stroke $c1 -strokewidth 20 -draw "stroke-linecap round bezier $points" \
	out.png
	//-fill $c1 -stroke none -draw "circle 500,500 510,500" \
	//-fill none -stroke $c2 -strokewidth 16 -draw "stroke-linecap round bezier $points" \
	//-fill $c2 -stroke none -draw "circle 500,500 508,500" \

	printf("DEBUG: Calling the script under the hood\n");
	system("chmod +x cmd.sh");
	system("./cmd.sh");

	return 0;
}
