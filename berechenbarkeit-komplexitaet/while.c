#include<stdio.h>

int whileProg(int x1);

int main(){
  int textvalues[2] = {1, 1};
  int i;
  for(i = 0; i < 2; i ++){
    printf("Value: %i - result: %i\n", textvalues[i], whileProg(textvalues[i]));
  }
}

int whileProg(int x1){
  int x2, x4, x5, x6, x7, x0;

  int x3 = 1;
  while(x3 != 0){
    x2 = x2 +1;
    x4 = 0;
    x5 = x2;
    while(x5 != 0){
      x4 = x4 + x2;
      x5 = x5 - 1;
    }
    x6 = x4 - x1;
    if(x6 < 0) x6 = 0;
    x7 = x1 - x4;
    if(x7 < 0) x7 = 0;
    x3 = x6 + x7;
  }
  x0 = x2;
  return x0;
}
