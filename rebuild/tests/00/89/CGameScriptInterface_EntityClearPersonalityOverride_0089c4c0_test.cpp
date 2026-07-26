#include <cstdio>

struct Entry { int key; void* obj; };
struct EntVec { Entry* begin; Entry* end; };
struct Ent { char pad[0x28]; int field28; char pad2[0x18]; EntVec vec; };
struct IGSI { void** vt; };
struct KeyPtr { int* p; };

static int g_released = 0;
static Entry g_arr[3];

Entry* __fastcall EntVec_LowerBound(EntVec* self, KeyPtr key) {
    for (Entry* p = self->begin; p != self->end; ++p)
        if (p->key >= *key.p) return p;
    return self->end;
}
void __fastcall Obj_Release(void* self) { if (self) g_released++; }

static Ent g_ent;
static Ent* __fastcall getent(IGSI* self) { (void)self; return &g_ent; }

void __stdcall CGameScriptInterface_EntityClearPersonalityOverride(IGSI* self)
{
    Ent* e = ((Ent* (__fastcall*)(IGSI*))self->vt[0x2c/4])(self);
    if (!e) return;
    if (!(e->field28 & 0x8000)) return;
    int local = 0x4f;
    KeyPtr k; k.p = &local;
    EntVec* v = &e->vec;
    Entry* r = EntVec_LowerBound(v, k);
    Entry* end = v->end;
    if (r != end && r->key <= 0x4f) {
    } else {
        r = end;
    }
    Obj_Release(r->obj);
}

int main() {
    void* vt[12];
    for (int i=0;i<12;i++) vt[i]=0;
    vt[11] = (void*)&getent;
    IGSI self; self.vt = vt;

    g_ent.field28 = 0x8000;
    g_arr[0].key = 0x4f; g_arr[0].obj = (void*)1;
    g_ent.vec.begin = &g_arr[0]; g_ent.vec.end = &g_arr[1];
    g_released = 0;
    CGameScriptInterface_EntityClearPersonalityOverride(&self);
    if (g_released != 1) { std::printf("FAIL released=%d\n", g_released); return 1; }

    g_ent.field28 = 0;
    g_released = 0;
    CGameScriptInterface_EntityClearPersonalityOverride(&self);
    if (g_released != 0) { std::printf("FAIL early\n"); return 1; }

    g_ent.field28 = 0x8000;
    g_arr[0].key = 0x50; g_arr[0].obj = (void*)1;
    g_ent.vec.end = &g_arr[1];
    g_arr[1].obj = 0;
    g_released = 0;
    CGameScriptInterface_EntityClearPersonalityOverride(&self);
    if (g_released != 0) { std::printf("FAIL endcase\n"); return 1; }

    std::printf("CGameScriptInterface_0089c4c0_TEST PASS\n");
    return 0;
}