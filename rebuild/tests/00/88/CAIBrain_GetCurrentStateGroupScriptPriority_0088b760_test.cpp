#include "rebuild_abi.h"
#include <cstdio>
#include <cstring>
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroupBase.h"

struct CAIBrain_Methods : CAIBrain {
    int GetCurrentStateGroupScriptPriority();
};

static void SetCurrent(CAIBrain_Methods& b, CAIStateGroupBase* s)
{
    *(CAIStateGroupBase**)&b.StateGroupList[0x10] = s;
}

int main(){
 CAIStateGroupBase st; std::memset(&st, 0, sizeof(st));
 int prio = 42; std::memcpy(st.ScriptAIPriority, &prio, 4); st.Terminated = false;
 CAIBrain_Methods b; std::memset(&b, 0, sizeof(b)); SetCurrent(b, &st);
 if(b.GetCurrentStateGroupScriptPriority()!=42){std::printf("B1\n");return 1;}
 st.Terminated = true; if(b.GetCurrentStateGroupScriptPriority()!=0){std::printf("B2\n");return 1;}
 SetCurrent(b, 0); if(b.GetCurrentStateGroupScriptPriority()!=0){std::printf("B3\n");return 1;}
 std::printf("GP_88b760_OK\n"); return 0; }
