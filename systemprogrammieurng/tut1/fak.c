#include <stdio.h>
#include <stdlib.h>

int fak(int n){
	int i = 1;
	int result = 1;
	for(i; i <= n; i++){
		result *= i;
	}
	printf("Das Ergebnis is %d\n", result);
	return result;
}
int recursiveFak(int n){
	return (n < 1)? n * recursiveFak(n - 1) : 1;
}

int main(int argc, char *argv[]){
	fak(atoi(argv[1]));
	return 0;
}

