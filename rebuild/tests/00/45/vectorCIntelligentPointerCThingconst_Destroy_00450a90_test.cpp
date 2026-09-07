#include <cstdio>
static int Count(unsigned char* first,unsigned char* last,unsigned int stride){int n=0;while(first!=last){++n;first+=stride;}return n;}
int main(){unsigned char a[0x54*3];if(Count(a,a+sizeof(a),0x54)!=3)return 1;puts("DESTROY_450A90_PASS");return 0;}