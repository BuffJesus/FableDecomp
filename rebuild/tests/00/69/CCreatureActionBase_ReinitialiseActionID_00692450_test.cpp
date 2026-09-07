#include "engine/CCreatureActionBase.h"
#include <cstdio>
static unsigned long g_actionid=100;
static void f(CCreatureActionBase* self){ self->ActionID=g_actionid; ++g_actionid; }
int main(){ CCreatureActionBase o; f(&o); if(o.ActionID!=100||g_actionid!=101){ std::printf("00692450_TEST FAIL\n"); return 1;}
 std::printf("00692450_TEST PASS\n"); return 0;}