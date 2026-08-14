#include <cstdio>

struct CThing;

struct NameObjVtbl {
    void* slot00; void* slot04; void* slot08; void* slot0c;
    void* slot10; void* slot14; void* slot18; void* slot1c;
    void* slot20; void* slot24;
    void* (__fastcall* slot28)(void* self, void* edx, CThing** ppThing, void* nameKey);
};
struct NameObj { NameObjVtbl* vtbl; };
struct CThing { char pad[0x70]; NameObj* nameObj; };
struct Filter { void* nameKey; };

static void* g_seen_ppThing_deref;
static void* g_seen_key;
static void* g_vret;

void* __fastcall stub_slot28(void* self, void* edx, CThing** ppThing, void* nameKey)
{
    (void)self; (void)edx;
    g_seen_ppThing_deref = *ppThing;
    g_seen_key = nameKey;
    return g_vret;
}

void* g_helper_arg;
bool helper_4b7980_model(void* ecx) { g_helper_arg = ecx; return ecx != 0; }

bool op_model(Filter* self, CThing* thing)
{
    void* key = self->nameKey;
    NameObj* obj = thing->nameObj;
    void* r = obj->vtbl->slot28(obj, 0, &thing, key);
    return helper_4b7980_model(r);
}

int main()
{
    NameObjVtbl vt; vt.slot28 = stub_slot28;
    NameObj obj; obj.vtbl = &vt;
    CThing thing; thing.nameObj = &obj;
    Filter f; f.nameKey = (void*)0xDEAD;

    g_vret = (void*)0x1234;
    bool r1 = op_model(&f, &thing);

    bool ok = true;
    if (g_seen_ppThing_deref != &thing) ok = false;
    if (g_seen_key != (void*)0xDEAD) ok = false;
    if (g_helper_arg != (void*)0x1234) ok = false;
    if (r1 != true) ok = false;

    g_vret = (void*)0;
    bool r2 = op_model(&f, &thing);
    if (r2 != false) ok = false;

    if (ok) printf("PARITY_OK_6a7c00\n");
    else printf("FAIL\n");
    return 0;
}