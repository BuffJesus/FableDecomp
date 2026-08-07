#include <cstdio>
static void* ga=0; static void* gb=0; static int gc=0;
static void ConstructStringHelper(void* a, void* b, int c){ ga=a; gb=b; gc=c; }
static void __fastcall ConstructString(void* a, void* b){ ConstructStringHelper(a, b, -1); }
int main(){ int x,y; ConstructString(&x,&y);
 if(ga!=&x||gb!=&y||gc!=-1){ std::printf("009c7b70_TEST FAIL\n"); return 1; }
 std::printf("009c7b70_TEST PASS\n"); return 0; }