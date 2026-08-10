#include <cstdio>
static int subCalled = 0, baseCalled = 0;
struct Sub { char pad[8]; };
struct Base { char pad[8]; };
struct Obj { char pad[0x114]; Sub sub; };
void SubDtor(Sub* s){ subCalled++; }
void BaseDtor(Base* b){ baseCalled++; }
void DestVal_114_C(Obj* self){ SubDtor(&self->sub); BaseDtor((Base*)self); }
int main(){
  Obj o;
  DestVal_114_C(&o);
  if(subCalled==1 && baseCalled==1){ printf("OK_0094e5d0\n"); return 0; }
  printf("FAIL\n"); return 1;
}