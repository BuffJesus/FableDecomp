#include <stdio.h>
struct M{float e[12];};int main(){M m={{0}};m.e[11]=7;if(m.e[11]!=7)return 1;printf("MESH_TRANSFORMATION_WRITE PASS\n");return 0;}