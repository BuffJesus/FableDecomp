#include <cstdio>
struct CCharString { const char* s; int n; void Ctor(const char* a,int b){ s=a; n=b; } };
CCharString* GetActionName(CCharString* ret){ ret->Ctor("DoCreatureAction",-1); return ret; }
int main(){
  CCharString slot; slot.s=0; slot.n=0;
  CCharString* r = GetActionName(&slot);
  int ok = (r==&slot) && (r->n==-1) && (r->s!=0);
  if(ok) printf("OK_004d4232\n");
  else printf("FAIL\n");
  return ok?0:1;
}