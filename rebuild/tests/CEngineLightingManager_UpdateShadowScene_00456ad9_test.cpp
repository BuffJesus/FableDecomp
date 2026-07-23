#include <cstdio>
#include <cstring>

static int g_guard=0, g_case2=0, g_case3=0, g_case3arg=0;

struct LM_Local { void build(void* obj24, void* arg){ g_case2=1; } };
struct LM_Obj28 { void apply(int arg){ g_case3=1; g_case3arg=arg; } };

struct CEngineLightingManager {
    char pad0[0x18];
    int mode;
    char pad1c[0x24-0x1c];
    void* obj24;
    LM_Obj28* obj28;
};

void __stdcall LM_Guard(const char* name){ g_guard=1; }

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, void** arg)
{
    int local;
    LM_Guard((const char*)0x122d70e);
    local = 0;
    if (self->mode == 2) {
        ((LM_Local*)&local)->build(self->obj24, arg);
    } else if (self->mode == 3) {
        self->obj28->apply(*(int*)arg);
    }
}

int main(){
    CEngineLightingManager o; memset(&o,0,sizeof(o));
    LM_Obj28 obj; o.obj28 = &obj;
    int payload = 0x1234;
    void* arg = &payload;               /* arg holds &payload */
    int expected = (int)(&payload);     /* *(int*)arg == &payload */

    /* mode 3 -> case3 with *arg */
    o.mode = 3;
    g_guard=g_case2=g_case3=g_case3arg=0;
    CEngineLightingManager_UpdateShadowScene(&o, 0, (void**)&arg);
    if(!(g_guard==1 && g_case3==1 && g_case2==0 && g_case3arg==expected)){ printf("FAIL mode3\n"); return 1; }

    /* mode 2 -> case2 */
    o.mode = 2;
    g_guard=g_case2=g_case3=0;
    CEngineLightingManager_UpdateShadowScene(&o, 0, (void**)&arg);
    if(!(g_guard==1 && g_case2==1 && g_case3==0)){ printf("FAIL mode2\n"); return 1; }

    /* mode other -> neither */
    o.mode = 5;
    g_guard=g_case2=g_case3=0;
    CEngineLightingManager_UpdateShadowScene(&o, 0, (void**)&arg);
    if(!(g_guard==1 && g_case2==0 && g_case3==0)){ printf("FAIL modeother\n"); return 1; }

    printf("CEngineLightingManager_00456ad9_TEST PASS\n");
    return 0;
}