#include <cstdio>
static int Count(unsigned char* first,unsigned char* last){int n=0;while(first!=last){first+=0xc4;++n;}return n;}
int main(){unsigned char a[0xc4*2];if(Count(a,a+sizeof(a))!=2)return 1;puts("DESTROY_450B1B_PASS");return 0;}