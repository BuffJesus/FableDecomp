#include <cstdio>
static void* g=0; static void __fastcall olc_target(void* p){ g=p; }
static void OnLeftClicked(void* self){ olc_target(self); }
int main(){ int m; OnLeftClicked(&m); if(g!=&m){ std::printf("00557850_TEST FAIL\n"); return 1;}
 std::printf("00557850_TEST PASS\n"); return 0;}