#include <cstdio>

static int g_slot0_calls = 0;
static int g_slot48_calls = 0;
static int g_arg0 = -1;
static int g_arg48 = -1;

struct ICanvas {
    virtual void slot0(int a) { g_slot0_calls++; g_arg0 = a; }
    virtual void v1() {} virtual void v2() {} virtual void v3() {}
    virtual void v4() {} virtual void v5() {} virtual void v6() {}
    virtual void v7() {} virtual void v8() {} virtual void v9() {}
    virtual void v10() {} virtual void v11() {} virtual void v12() {}
    virtual void v13() {} virtual void v14() {} virtual void v15() {}
    virtual void v16() {} virtual void v17() {} virtual void v18() {}
    virtual void v19() {} virtual void v20() {} virtual void v21() {}
    virtual void v22() {} virtual void v23() {} virtual void v24() {}
    virtual void v25() {} virtual void v26() {} virtual void v27() {}
    virtual void v28() {} virtual void v29() {} virtual void v30() {}
    virtual void v31() {} virtual void v32() {} virtual void v33() {}
    virtual void v34() {} virtual void v35() {} virtual void v36() {}
    virtual void v37() {} virtual void v38() {} virtual void v39() {}
    virtual void v40() {} virtual void v41() {} virtual void v42() {}
    virtual void v43() {} virtual void v44() {} virtual void v45() {}
    virtual void v46() {} virtual void v47() {}
    virtual void slot48(int a) { g_slot48_calls++; g_arg48 = a; }
};

struct CManager {
    char pad[0x9c];
    char flag9c;
    char pad2[3];
    ICanvas* canvas;
};

void __fastcall CManager_ClearErrorMessage(CManager* self)
{
    ICanvas* c = self->canvas;
    self->flag9c = 0;
    c->slot48(0);
    c = self->canvas;
    if (c) {
        c->slot0(1);
    }
    self->canvas = 0;
}

int main()
{
    ICanvas canvas;
    CManager mgr;
    mgr.flag9c = 1;
    mgr.canvas = &canvas;

    CManager_ClearErrorMessage(&mgr);

    int ok = 1;
    if (mgr.flag9c != 0) ok = 0;
    if (g_slot48_calls != 1 || g_arg48 != 0) ok = 0;
    if (g_slot0_calls != 1 || g_arg0 != 1) ok = 0;
    if (mgr.canvas != 0) ok = 0;

    if (ok) { std::printf("CManager_0041ce6a_TEST PASS\n"); return 0; }
    std::printf("CManager_0041ce6a_TEST FAIL f=%d s48=%d(%d) s0=%d(%d) cv=%p\n",
        mgr.flag9c, g_slot48_calls, g_arg48, g_slot0_calls, g_arg0, mgr.canvas);
    return 1;
}