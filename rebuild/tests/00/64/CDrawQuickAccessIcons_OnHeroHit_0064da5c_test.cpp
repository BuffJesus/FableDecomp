#include <cstdio>
struct OH { char pad[0x2c]; int f2c; char pad2[0x34-0x30]; char f34; };
static OH* gS;
void __fastcall h_0064da5c(OH* self){ gS=self; }
void __fastcall fn_0064da5c(OH* self){ h_0064da5c(self); self->f2c = -1; self->f34 = 0; }
int main(){ OH o; o.f2c=0; o.f34=7; fn_0064da5c(&o);
 if(gS!=&o||o.f2c!=-1||o.f34!=0){ printf("0064da5c_TEST FAIL\n"); return 1; }
 printf("0064da5c_TEST PASS\n"); return 0; }