#include <cstdio>
static int g_order[8]; static int g_n=0; static void* g_self;
struct Obj { int tag; };
void __fastcall sub_A(void* self){ g_order[g_n++]=1; g_self=self; }
void __fastcall sub_B(void* self){ g_order[g_n++]=2; }
void __fastcall sub_C(void* self){ g_order[g_n++]=3; }
void __fastcall sub_D(void* self){ g_order[g_n++]=4; }
void __fastcall Dest_val_allocator(void* self){ sub_A(self); sub_B(self); sub_C(self); sub_D(self); }
int main(){ Obj o; o.tag=99; Dest_val_allocator(&o);
  int ok=(g_n==4)&&g_order[0]==1&&g_order[1]==2&&g_order[2]==3&&g_order[3]==4&&(g_self==&o);
  printf(ok?"DESTVAL_OK\n":"DESTVAL_FAIL\n"); return 0; }