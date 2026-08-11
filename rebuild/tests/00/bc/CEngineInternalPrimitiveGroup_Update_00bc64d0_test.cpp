#include <cstdio>

struct CEnginePrimitive {
    int pad0; int pad4; int pad8; int fieldC;
};

static int g_lastAdd;
static void* g_lastThis;

struct CInner {
    void Add(int v);
};

struct CEngineInternalPrimitiveGroup {
    char pad_0[0x50];
    CInner inner;
    char pad_54[0x5c-0x54];
    unsigned long m_5c;

    void Update(const CEnginePrimitive& prim, unsigned long a3);
};

// provide the extern the source calls
void CInner::Add(int v) { g_lastAdd = v; g_lastThis = this; }

int main() {
    CEngineInternalPrimitiveGroup g;
    CEnginePrimitive p;
    p.fieldC = 0x1234;
    g.Update(p, 0xABCDu);

    bool ok = true;
    if (g.m_5c != 0xABCDu) ok = false;
    if (g_lastAdd != 0x1234) ok = false;
    if (g_lastThis != (void*)&g.inner) ok = false;
    if ((char*)&g.inner - (char*)&g != 0x50) ok = false;

    printf(ok ? "PARITY_OK_00bc64d0\n" : "FAIL\n");
    return ok ? 0 : 1;
}