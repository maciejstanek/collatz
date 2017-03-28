#include <cstdlib>
#include <cstdio>
#include <vector>
#include <iostream>

#define DEBUG

using namespace std;

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Collatz Conjecture Visualizer\n");
		printf("Usage: ./collatz <limit>\n");
		printf(" <limit> - the top number to visualize\n");
		return 1;
	}

	int limit = atoi(argv[1]);
	#ifdef DEBUG
	printf("MAX=%d\n", limit);
	#endif

	vector<int> jumps(limit+1, 0);
	for(vector<int>::iterator i = jumps.begin(); i != jumps.end(); i++) {
		int index = distance(jumps.begin(), i);
		if(index == 0) {
			*i = -1;
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

	#ifdef DEBUG
	for(vector<int>::iterator i = jumps.begin(); i != jumps.end(); i++) {
		int index = distance(jumps.begin(), i);
		printf("(%d, %d)\n", index, *i);
	}
	#endif

	vector<vector<int>*> paths;

	for(vector<int>::reverse_iterator i = jumps.rbegin(); i != jumps.rend(); i++) {
		int index = distance(jumps.begin(), i.base()) - 1;
		cout << index << endl;
	}

	return 0;
}
