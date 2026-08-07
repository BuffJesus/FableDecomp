#include <cstdio>
struct That { void m(int id); };
struct CM { char pad[0x4]; That* f4; char pad2[0x54-0x8]; char f54; };
static void* gW; static int gId;
void That::m(int id){ gW=this; gId=id; }
void __fastcall fn_0079c640(CM* self){ self->f54 = 0; self->f4->m(0x92); }
int main(){ CM c; c.f54=1; c.f4=(That*)0x77; fn_0079c640(&c);
 if(c.f54!=0||gW!=(void*)0x77||gId!=0x92){ printf("0079c640_TEST FAIL\n"); return 1; }
 printf("0079c640_TEST PASS\n"); return 0; }