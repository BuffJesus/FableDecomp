#include <cstdio>
struct Inner { int called; int arg; };
struct CTCBulletTime { Inner* f4; char pad[0x54-8]; char f54; char pad2; char f56; };
Inner g_inner;
void Inner_Method(Inner* self, int a) { self->called=1; self->arg=a; }
void model(CTCBulletTime* self){ self->f56=1; self->f54=0; Inner_Method(self->f4, 0x92); }
int main(){
  CTCBulletTime o; o.f4=&g_inner; o.f54=9; o.f56=9; g_inner.called=0; g_inner.arg=0;
  model(&o);
  if(o.f56!=1){printf("FAIL f56\n");return 1;}
  if(o.f54!=0){printf("FAIL f54\n");return 1;}
  if(g_inner.called!=1||g_inner.arg!=0x92){printf("FAIL call\n");return 1;}
  printf("OK_0079c660\n");return 0;
}