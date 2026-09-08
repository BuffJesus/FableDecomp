#include <stdio.h>
struct B{float a,b,c;};int main(){B x={1,2,3},y={4,5,6};x=y;if(x.a!=4||x.b!=5||x.c!=6)return 1;printf("BALVERINE_BATTLE_ASSIGN_00478876 PASS\\n");return 0;}