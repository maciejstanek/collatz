#include <cstdlib>
#include <cstdio>

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Collatz Conjecture Visualizer\n");
		printf("Usage: ./collatz <limit>\n");
		printf(" <limit> - the top number to visualize\n");
		return 1;
	}
	return 0;
}
