#include <cstdio>
static long g_x=-1; static int g_z=-1;
static void __stdcall fl_helper(long x, int z){ g_x=x; g_z=z; }
struct CWorldMap { void ForceLoad(long x); };
void CWorldMap::ForceLoad(long x){ fl_helper(x, 0); }
int main(){ CWorldMap o; o.ForceLoad(77);
 if(g_x!=77||g_z!=0){ std::printf("005063d0_TEST FAIL\n"); return 1;}
 std::printf("005063d0_TEST PASS\n"); return 0;}