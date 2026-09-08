#include <stdio.h>
struct E{long X,Y;};int main(){unsigned long width=640,height=480;E e={(long)width,(long)height};if(e.X!=640||e.Y!=480)return 1;printf("SURFACE_DIMENSIONS PASS\n");return 0;}