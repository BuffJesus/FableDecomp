#include <stdio.h>
struct V{float x,y,z;};int main(){V a={0,0,0},b={1,-2,3};a=b;if(a.x!=1||a.y!=-2||a.z!=3)return 1;printf("VECTOR_STRING_PAIR_ASSIGN PASS\n");return 0;}