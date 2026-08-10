#include <cstdio>

struct Sub { int x; };
struct Obj { char pad[0x114]; Sub sub; };

static int g_subCalled = 0;
static int g_otherCalled = 0;
static Sub* g_subPtr = 0;
static Obj* g_otherThis = 0;

void SubMethod(Sub* s) { g_subCalled++; g_subPtr = s; }
void ObjOther(Obj* o) { g_otherCalled++; g_otherThis = o; }

// Behavioural model of DestVal_Music_B: calls sub method on subobject at +0x114,
// then tail-calls Obj_Other with the full this pointer.
void Model(Obj* self) {
    SubMethod(&self->sub);
    ObjOther(self);
}

int main() {
    Obj obj;
    Model(&obj);
    if (g_subCalled != 1) { printf("FAIL sub\n"); return 1; }
    if (g_otherCalled != 1) { printf("FAIL other\n"); return 1; }
    if ((char*)g_subPtr != (char*)&obj + 0x114) { printf("FAIL suboff\n"); return 1; }
    if (g_otherThis != &obj) { printf("FAIL otherthis\n"); return 1; }
    printf("OK_0066a4f0\n");
    return 0;
}