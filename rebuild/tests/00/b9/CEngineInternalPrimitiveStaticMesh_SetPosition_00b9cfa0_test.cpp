#include <cstdio>

struct CEnginePrimitivePosition {
    char pad[0xc];
    int payload;
};

static void* g_self;
static const CEnginePrimitivePosition* g_rawPos;
static const void* g_payloadPtr;
static int g_field58;

void __fastcall CEngineInternalPrimitiveStaticMesh_Sub_Apply(
        void* self, const CEnginePrimitivePosition* rawPos,
        const void* payloadPtr, int field58) {
    g_self = self; g_rawPos = rawPos; g_payloadPtr = payloadPtr; g_field58 = field58;
}

struct CEngineInternalPrimitiveStaticMesh {
    char pad0[0x58];
    int field58;
    char pad1[0x70 - 0x58 - 4];
    char sub;
};

void __fastcall CEngineInternalPrimitiveStaticMesh_SetPosition(
        CEngineInternalPrimitiveStaticMesh* self,
        int /*edx*/,
        const CEnginePrimitivePosition& pos);

int main() {
    CEngineInternalPrimitiveStaticMesh obj;
    obj.field58 = 0x12345678;
    CEnginePrimitivePosition pos;
    pos.payload = 0x99;

    CEngineInternalPrimitiveStaticMesh_SetPosition(&obj, 0, pos);

    bool ok = (g_self == (void*)&obj.sub)
           && (g_rawPos == &pos)
           && (g_payloadPtr == (const void*)&pos.payload)
           && (g_field58 == 0x12345678);
    if (ok) printf("00b9cfa0_TEST PASS\n");
    else printf("FAIL self=%p sub=%p rp=%p pos=%p pptr=%p payl=%p f=%x\n",
                g_self,(void*)&obj.sub,(void*)g_rawPos,(void*)&pos,
                g_payloadPtr,(void*)&pos.payload,g_field58);
    return 0;
}