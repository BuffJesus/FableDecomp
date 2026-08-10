#include <cstdio>
struct Obj { char pad[0x1c]; int field_1c; int helperCalls; int tailCalls; };
static void Helper(Obj* s){ s->helperCalls++; }
static void Tail(Obj* s){ s->tailCalls++; }
static void Proc(Obj* s){ if (s->field_1c == 1) Helper(s); Tail(s); }
int main(){
  Obj a; a.field_1c=1; a.helperCalls=0; a.tailCalls=0;
  Proc(&a);
  if (a.helperCalls!=1 || a.tailCalls!=1) { printf("FAIL\n"); return 1; }
  Obj b; b.field_1c=0; b.helperCalls=0; b.tailCalls=0;
  Proc(&b);
  if (b.helperCalls!=0 || b.tailCalls!=1) { printf("FAIL\n"); return 1; }
  Obj c; c.field_1c=2; c.helperCalls=0; c.tailCalls=0;
  Proc(&c);
  if (c.helperCalls!=0 || c.tailCalls!=1) { printf("FAIL\n"); return 1; }
  printf("OK_00576c39\n");
  return 0;
}