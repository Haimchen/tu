#include <stdio.h>
#include <stdlib.h>

int isPalim(char* str){
	int l = 0;
	int r = 0;				//Laenge des Strings bestimmen
	while(str[r] != 0){
		r++;
	}
	r = r - 1;
	for (l; l <= r/2; ++l)
	{
		if(str[l] != str[r-l]){
			return 0;
		}
	}
	return 1;
}

int main(int argc, char *argv[]){
	char str[] = "rentn";
	printf("Das Wort ist %s Palindrom!\n", isPalim(&str[0]) == 1 ? "ein" : "kein");
	return 0;
}