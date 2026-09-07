#include <stdio.h>
#include <string.h>
static unsigned char* Copy(unsigned char* b,unsigned char* e,unsigned char* d){unsigned n=e-b;if(n){memmove(d,b,n);d+=n;}return d;}
int main(){unsigned char a[8]={1,2,3,4,5,6,7,8};unsigned char b[8]={0};if(Copy(a,a,b)!=b)return 1;if(Copy(a,a+5,b)!=b+5||b[4]!=5)return 2;memmove(a+1,a,6);if(a[1]!=1||a[6]!=6)return 3;puts("UNSEEN_SMALL_PASS");return 0;}