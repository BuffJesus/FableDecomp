#include <cstdio>
struct CEngineLightingManager { char pad0[0x18]; int mode; char pad1c[8]; void* obj24; void* obj28; };
int g_log=0, g_do=0, g_push=0; void* g_pushArg=0; void* g_doThis=0;
void run(CEngineLightingManager* self, void* param){
  int local;
  g_log++;
  local = 0;
  switch(self->mode){
  case 2:
    g_do++; g_doThis=&local; (void)self->obj24; (void)param; break;
  case 3:
    g_push++; g_pushArg = *(void**)param; (void)self->obj28; break;
  }
  (void)local;
}
int main(){
  CEngineLightingManager m; int inner=0x1234; void* pinner=&inner; void* p=&pinner;
  m.mode=2; g_log=g_do=g_push=0; run(&m,p);
  if(!(g_log==1&&g_do==1&&g_push==0&&g_doThis!=0)){ std::printf("FAIL mode2\n"); return 1; }
  m.mode=3; g_log=g_do=g_push=0; run(&m,p);
  if(!(g_log==1&&g_do==0&&g_push==1&&g_pushArg==pinner)){ std::printf("FAIL mode3\n"); return 1; }
  m.mode=5; g_log=g_do=g_push=0; run(&m,p);
  if(!(g_log==1&&g_do==0&&g_push==0)){ std::printf("FAIL default\n"); return 1; }
  std::printf("CEngineLightingManager_00456c21_TEST PASS\n");
  return 0;
}