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
	if(argc != 4) {
		printf("Collatz Conjecture Visualizer\n");
		printf("Usage: ./collatz <limit> <angle> <step>\n");
		printf(" | <limit> - the top number to visualize\n");
		printf(" | <angle> - angle per each iteration\n");
		printf(" | <step>  - step per each iteration");
		return 1;
	}
	
	
	int limit = atoi(argv[1]);
	double angleChange = atof(argv[2]);
	double step = atof(argv[3]);

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

	for(vector<vector<int>*>::iterator i = paths.begin(); i != paths.end(); i++) {
		if((*i)->size() < 2 || (*i)->back() != 0) {
			delete (*i);
			paths.erase(i--);
		}
	}

	vector<vector<Vect*>*> physicalPaths;

	for(vector<vector<int>*>::reverse_iterator i = paths.rbegin(); i != paths.rend(); i++) {
		physicalPaths.push_back(new vector<Vect*>);
		physicalPaths.back()->push_back(new Vect(0, 0, M_PI));
		for(vector<int>::reverse_iterator j = (*i)->rbegin() + 1; j != (*i)->rend(); j++) {
			printf("%d ", *j);
			Vect *newVect;
			newVect = physicalPaths.back()->back()->getNewNext(step, ((*j)%2?-1:1)*angleChange);
			physicalPaths.back()->push_back(newVect);
		}
		printf("\n");
	}
	int imgSize = 2200;
	FILE *fp;
	fp = fopen("cmd.sh", "w");
	fprintf(fp, "#!/bin/bash\n");
	fprintf(fp, "c1=#45961b\n");
	fprintf(fp, "c2=#67bf39\n");
	fprintf(fp, "convert -size %dx%d xc:white \\\n", imgSize, imgSize);

	for(vector<vector<Vect*>*>::iterator i = physicalPaths.begin(); i != physicalPaths.end(); i++) {
		fprintf(fp, "\t-fill none -stroke black -draw \"polyline");
		for(vector<Vect*>::iterator j = (*i)->begin(); j != (*i)->end(); j++) {
			fprintf(fp, " %d,%d", imgSize/2+(int)round((*j)->x), imgSize/2+(int)round((*j)->y));
		}
		fprintf(fp, "\" \\\n");
	}
	fprintf(fp, "\tout.png");
	fclose(fp);

	//-fill none -stroke $c1 -strokewidth 20 -draw "stroke-linecap round bezier $points" \
	out.png
	//-fill $c1 -stroke none -draw "circle 500,500 510,500" \
	//-fill none -stroke $c2 -strokewidth 16 -draw "stroke-linecap round bezier $points" \
	//-fill $c2 -stroke none -draw "circle 500,500 508,500" \

	system("chmod +x cmd.sh");
	system("./cmd.sh");

	return 0;
}
