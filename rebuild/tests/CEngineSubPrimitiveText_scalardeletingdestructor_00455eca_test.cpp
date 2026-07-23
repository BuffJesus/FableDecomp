#include <cstdio>

struct CEngineSubPrimitiveText { char pad[0x58]; };

static int g_sub54 = 0;
static int g_real = 0;
static int g_del = 0;

extern "C" void __fastcall Sub54_dtor(void* ecx) { (void)ecx; g_sub54++; }
extern "C" void __fastcall CEngineSubPrimitiveText_realdtor(void* ecx) { (void)ecx; g_real++; }
void operator delete(void* p) { (void)p; g_del++; }

void* __fastcall CEngineSubPrimitiveText_scalar_deleting_destructor(CEngineSubPrimitiveText* self, int edx, unsigned int flags)
{
    (void)edx;
    Sub54_dtor((char*)self + 0x54);
    CEngineSubPrimitiveText_realdtor(self);
    if (flags & 1)
        operator delete(self);
    return self;
}

int main()
{
    CEngineSubPrimitiveText obj;
    // flags bit0 clear: no delete
    g_sub54 = g_real = g_del = 0;
    void* r = CEngineSubPrimitiveText_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_sub54 != 1 || g_real != 1 || g_del != 0) { std::printf("FAIL nodelete s=%d r=%d d=%d\n", g_sub54, g_real, g_del); return 1; }
    // flags bit0 set: delete
    CEngineSubPrimitiveText* heap = new CEngineSubPrimitiveText();
    g_sub54 = g_real = g_del = 0;
    r = CEngineSubPrimitiveText_scalar_deleting_destructor(heap, 0, 1);
    if (r != heap || g_sub54 != 1 || g_real != 1 || g_del != 1) { std::printf("FAIL delete s=%d r=%d d=%d\n", g_sub54, g_real, g_del); return 1; }
    std::printf("CEngineSubPrimitiveText_00455eca_TEST PASS\n");
    return 0;
}