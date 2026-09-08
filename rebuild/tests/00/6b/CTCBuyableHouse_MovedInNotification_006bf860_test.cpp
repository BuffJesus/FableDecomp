#include <stdio.h>
static long M(long a,long b){return a<b?a:b;}int main(){long day=20;day=M(day,6);if(day!=6)return 1;printf("BUYABLE_HOUSE_MOVED_IN PASS\n");return 0;}