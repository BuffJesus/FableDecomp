#include <cstdio>
static int calls=0;
struct Sub{int x;};
struct Base{int x;};
static void Sub_dtor(Sub* s){ if(s) calls|=1; }
static void Base_dtor(Base* b){ if(b) calls|=2; }
struct Obj{ char pad[0xa8]; Sub sub; };
static void model(Obj* self){ Sub_dtor(&self->sub); Base_dtor((Base*)self); }
int main(){
  Obj o; model(&o);
  int c2=0; Sub* np=0; if(np) c2|=1; if(!np) c2|=2;
  if(calls==3 && c2==2){ printf("OK_007eb010 %d\n", c2); return 0; }
  printf("FAIL %d %d\n", calls, c2); return 1;
}