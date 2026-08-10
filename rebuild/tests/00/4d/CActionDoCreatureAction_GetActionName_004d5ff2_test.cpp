#include <cstdio>
struct CCharString {
  char* p;
  const char* seen_s; int seen_n; bool called;
  CCharString* ctorB(const char* s, int n){ seen_s=s; seen_n=n; called=true; return this; }
  CCharString* GetActionName_8(){ ctorB((const char*)0x123dd2c, -1); return this; }
};
int main(){
  CCharString c; c.called=false; c.seen_s=0; c.seen_n=0;
  CCharString* r = c.GetActionName_8();
  if(r==&c && c.called && c.seen_s==(const char*)0x123dd2c && c.seen_n==-1)
    printf("OK_004d5ff2\n");
  else { printf("FAIL s=%p n=%d called=%d ret=%p\n", c.seen_s, c.seen_n, (int)c.called, (void*)r); return 1; }
  return 0;
}