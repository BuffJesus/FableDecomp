struct Entry { int key; void* obj; };

struct EntVec {
    Entry* begin;
    Entry* end;
};

struct Ent {
    char pad[0x28];
    int field28;      // +0x28
    char pad2[0x18];
    EntVec vec;       // +0x44
};

struct IGSI {
    Ent* (**vt)();
};

struct KeyPtr { int* p; };

extern Entry* __fastcall EntVec_LowerBound(EntVec* self, KeyPtr key);
extern void __fastcall Obj_Release(void* self);

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
        // keep r
    } else {
        r = end;
    }
    Obj_Release(r->obj);
}