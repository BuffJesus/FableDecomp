#include <cstdio>
double g_procLoadTime = 0.0;
struct CEngineResourceManager { char pad[0x10]; };
struct SubObj { void Do(int a, double d); };
static void* g_this=0; static int g_a=-1; static double g_d=0.0;
void SubObj::Do(int a, double d){ g_this=this; g_a=a; g_d=d; }
void __fastcall ProcessLoading(CEngineResourceManager* self);
int main(){
  char buf[0x40];
  CEngineResourceManager* self=(CEngineResourceManager*)(buf+0xc);
  g_procLoadTime=7.5;
  ProcessLoading(self);
  if(g_this==(void*)buf && g_a==0 && g_d==7.5) printf("00b3b8c0_TEST PASS\n");
  else printf("FAIL this=%p a=%d d=%f\n",g_this,g_a,g_d);
  return 0;
}