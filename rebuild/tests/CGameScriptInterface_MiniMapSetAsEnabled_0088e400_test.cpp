#include <cstdio>

struct Inner { int calls; bool last; };
struct CGameScriptInterface { void* pad0; Inner* inner; };

static void Inner_SetEnabled(Inner* p, bool b) { p->calls++; p->last = b; }

void CGameScriptInterface_MiniMapSetAsEnabled(CGameScriptInterface* self, bool enabled)
{
    Inner_SetEnabled(self->inner, enabled);
}

int main()
{
    Inner obj; obj.calls = 0; obj.last = false;
    CGameScriptInterface gsi; gsi.pad0 = 0; gsi.inner = &obj;
    CGameScriptInterface_MiniMapSetAsEnabled(&gsi, true);
    CGameScriptInterface_MiniMapSetAsEnabled(&gsi, false);
    if (obj.calls == 2 && obj.last == false) {
        std::printf("CGameScriptInterface_0088e400_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d last=%d\n", obj.calls, (int)obj.last);
    return 1;
}