#include <stdio.h>
static float f(float a,float b){return a<b?b:a;}int main(){if(f(4.0f,-2.0f)!=4.0f)return 1;printf("LINE_HIGHEST_Y PASS\\n");return 0;}