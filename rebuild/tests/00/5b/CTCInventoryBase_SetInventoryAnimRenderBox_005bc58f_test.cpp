#include <stdio.h>
struct B{float a,b,c,d;};int main(){B x={1,2,3,4},y={0,0,0,0};y=x;if(y.a!=1||y.d!=4)return 1;printf("INVENTORY_ANIM_RENDER_BOX PASS\n");return 0;}