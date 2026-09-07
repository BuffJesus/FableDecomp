#include <cstdio>
static int Clean(bool a,bool b){int n=0;if(b)++n;if(a)++n;return n;}
int main(){if(Clean(true,true)!=2||Clean(false,true)!=1||Clean(false,false)!=0)return 1;puts("PATCH_EDGE_DTOR_PASS");return 0;}