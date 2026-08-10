#include <cstdio>
#include <cstdlib>
static int g_freed = 0;
static void* g_last = 0;
struct CActiveFile { void* buf; int stepped; };
static void OtherStep(CActiveFile* s){ s->stepped=1; }
static void model(CActiveFile* self){
  OtherStep(self);
  if (self->buf){ g_freed++; g_last=self->buf; }
}
int main(){
  CActiveFile a; a.buf=(void*)0x1234; a.stepped=0;
  model(&a);
  if(!a.stepped) { printf("FAIL step\n"); return 1; }
  if(g_freed!=1 || g_last!=(void*)0x1234){ printf("FAIL free\n"); return 1; }
  CActiveFile b; b.buf=0; b.stepped=0;
  model(&b);
  if(!b.stepped){ printf("FAIL step2\n"); return 1; }
  if(g_freed!=1){ printf("FAIL nofree\n"); return 1; }
  printf("OK_005b2160\n");
  return 0;
}