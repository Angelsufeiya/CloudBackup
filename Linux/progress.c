#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	char arr[102] = {0};
	const char* lp = "|/-\\";
	for(int i = 0; i < 100; i++){
		arr[i] = '#';
		printf("[%-100s][%d%%][%c]\r", arr, i+1, lp[i % 4]);
		fflush(stdout);
		usleep(100000);
	}
	printf("\n");
	return 0;
}
