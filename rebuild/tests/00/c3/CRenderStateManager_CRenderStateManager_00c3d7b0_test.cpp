#include <cstdio>
struct RM { void* vptr; char pad[0x44]; unsigned int f48; void dtor(); };
static int gD; void RM::dtor(){ gD=1; }
void __fastcall fn_00c3d7b0(RM* self){ self->vptr=(void*)0x12b5a80; self->f48=0x80000000u; self->dtor(); }
int main(){ RM r; gD=0; fn_00c3d7b0(&r);
 if(r.vptr!=(void*)0x12b5a80||r.f48!=0x80000000u||gD!=1){ printf("00c3d7b0_TEST FAIL\n"); return 1; }
 printf("00c3d7b0_TEST PASS\n"); return 0; }