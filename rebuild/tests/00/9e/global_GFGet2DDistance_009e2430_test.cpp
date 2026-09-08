#include <stdio.h>
#include <math.h>
struct P{long X,Y;};float Distance(const P&a,const P&b){long x=a.X-b.X,y=a.Y-b.Y;return sqrtf(float(x*x+y*y));}
int main(){P a={0,0},b={3,4};if(Distance(a,b)!=5.0f)return 1;printf("GET_2D_DISTANCE PASS\n");return 0;}