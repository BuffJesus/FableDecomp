#include <cstdio>

struct CWorldMapField4Obj;

static void* g_capturedObj = 0;
static int g_capturedFlag = -1;

extern "C" void __stdcall HelperPrepare_7a7bf1(CWorldMapField4Obj* obj, int flag) {
    g_capturedObj = (void*)obj;
    g_capturedFlag = flag;
}

struct CWorldMap {
    void* field0;
    CWorldMapField4Obj* field4;

    void __fastcall PrepareForLoad();
};

void __fastcall CWorldMap::PrepareForLoad() {
    HelperPrepare_7a7bf1(field4, 0);
}

int main() {
    int dummy = 0;
    CWorldMap wm;
    wm.field0 = 0;
    wm.field4 = (CWorldMapField4Obj*)&dummy;

    wm.PrepareForLoad();

    if (g_capturedObj == (void*)&dummy && g_capturedFlag == 0) {
        printf("PREPAREFORLOAD_OK\n");
    } else {
        printf("PREPAREFORLOAD_FAIL obj=%p flag=%d\n", g_capturedObj, g_capturedFlag);
    }
    return 0;
}