#include <cstdio>
struct BW { char pad[0x14]; int f14; };
static BW* gS3;
void __fastcall h1_9143c0(BW* self){ gS3=self; }
int __cdecl h2_9143c0(){ return 88; }
void __fastcall fn_009143c0(BW* self){ h1_9143c0(self); self->f14 = h2_9143c0(); }
int main(){ BW b; fn_009143c0(&b); if(gS3!=&b||b.f14!=88){ printf("009143c0_TEST FAIL\n"); return 1; }
 printf("009143c0_TEST PASS\n"); return 0; }