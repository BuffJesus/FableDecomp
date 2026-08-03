#include <cstdio>

enum ECreatureGroup { CG_A = 7 };

struct RecordedCall {
    void* self;
    int group;
    bool enabled;
    bool called;
};

static RecordedCall g_rec = { 0, 0, false, false };

struct CWorld {
    int m_id;
    void __fastcall SetCreatureGenerationGroupAsEnabled(ECreatureGroup group, bool enabled) {
        g_rec.self = this;
        g_rec.group = (int)group;
        g_rec.enabled = enabled;
        g_rec.called = true;
    }
};

struct Overlay {
    char pad[4];
    CWorld* pWorld;
};

struct CGameScriptInterface {
    void __fastcall SetCreatureGeneratorsCreatureGroupAsEnabled(ECreatureGroup group, bool enabled) const {
        reinterpret_cast<const Overlay*>(this)->pWorld->SetCreatureGenerationGroupAsEnabled(group, enabled);
    }
};

int main() {
    CWorld world;
    world.m_id = 123;
    Overlay ov;
    ov.pWorld = &world;
    const CGameScriptInterface* gsi = reinterpret_cast<const CGameScriptInterface*>(&ov);
    gsi->SetCreatureGeneratorsCreatureGroupAsEnabled(CG_A, true);
    if (g_rec.called && g_rec.self == (void*)&world && g_rec.group == 7 && g_rec.enabled == true) {
        printf("THUNK_FORWARD_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}