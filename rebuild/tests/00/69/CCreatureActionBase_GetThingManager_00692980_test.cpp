#include <cstdio>
struct Sub; struct CThingManager; struct S { char pad[8]; };
static int g_ran=0; static CThingManager* g_thingmgr=(CThingManager*)0x1234;
static void gtm_sub(Sub* p){ (void)p; g_ran=1; }
static CThingManager* GetThingManager(S* self){ gtm_sub(reinterpret_cast<Sub*>(reinterpret_cast<char*>(self)+8)); return g_thingmgr; }
int main(){ S obj; CThingManager* r=GetThingManager(&obj);
 if(!g_ran||r!=(CThingManager*)0x1234){ std::printf("00692980_TEST FAIL\n"); return 1;}
 std::printf("00692980_TEST PASS\n"); return 0;}