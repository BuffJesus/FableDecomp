#include "rebuild_abi.h"
#include <cstdio>
struct Loc { int v; };
static int g_consumed=0;
extern "C" void FABLE_FASTCALL FableConsume_b59760(Loc *p){ g_consumed=p->v; }
struct CAIGameCameraBase {
    virtual void v0(){} virtual void v1(){} virtual void v2(){} virtual void v3(){}
    virtual void v4(){} virtual void v5(){} virtual void v6(){} virtual void v7(){}
    virtual void v8(){} virtual void GetSub(Loc *out){ out->v=99; }
    void GetControlAngleXY();
};
void CAIGameCameraBase::GetControlAngleXY(){ Loc local; GetSub(&local); FableConsume_b59760(&local); }
int main(){ CAIGameCameraBase o; o.GetControlAngleXY();
 if(g_consumed!=99){std::printf("B1\n");return 1;}
 std::printf("CT_b59760_OK\n"); return 0; }