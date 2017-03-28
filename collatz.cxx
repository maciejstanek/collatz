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
				x + step*sin(a+angleChange),
				y + step*sin(a+angleChange),
				a + angleChange);
		}
};

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Collatz Conjecture Visualizer\n");
		printf("Usage: ./collatz <limit>\n");
		printf(" <limit> - the top number to visualize\n");
		return 1;
	}

	int limit = atoi(argv[1]);

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
		for(vector<int>::reverse_iterator j = (*i)->rbegin(); j != (*i)->rend(); j++) {
			printf("%d ", *j);
		}
		printf("\n");
	}
		/*
		vector<> *list = new vector<int>;
		int next = index;
		list->push_back(next);
		while(jumps[next] >= 0) {
			#ifdef DEBUG
			printf("[%d][%d]\n", next, jumps[next]);
			#endif
			next = jumps[next];
			list->push_back(next);
		}
		#ifdef DEBUG
		printf("\n");
		#endif
		paths.push_back(list);
		*/
	//}

	return 0;
}
