#include <stdio.h>
static float f(float a,float b){return a<b?a:b;}int main(){if(f(4.0f,-2.0f)!=-2.0f)return 1;printf("LINE_LOWEST_Y PASS\\n");return 0;}