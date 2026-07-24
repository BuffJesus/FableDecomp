#include <cstdio>
struct CGameScriptInterface { unsigned char* vt; };
static int g_entity = 0;
static int g_max = 0;
static int g_called = 0;
static void* g_this = 0;
__declspec(naked) void stub_method()
{
    __asm {
        mov g_this, ecx
        mov eax, dword ptr [esp+4]
        mov g_entity, eax
        mov eax, dword ptr [esp+8]
        mov g_max, eax
        mov dword ptr g_called, 1
        ret 8
    }
}
__declspec(naked) void __fastcall CGameScriptInterface_EntityClearMaxNumberOfAttackers(CGameScriptInterface* self, void* edx_ignore, int entity)
{
    __asm {
        mov edx, dword ptr [esp+4]
        mov eax, dword ptr [ecx]
        push -1
        push edx
        call dword ptr [eax+0x73c]
        ret 4
    }
}
int main()
{
    static void* vtable[0x73c/4 + 1];
    vtable[0x73c/4] = (void*)&stub_method;
    CGameScriptInterface obj;
    obj.vt = (unsigned char*)vtable;
    CGameScriptInterface_EntityClearMaxNumberOfAttackers(&obj, 0, 12345);
    if (g_called == 1 && g_entity == 12345 && g_max == -1 && g_this == &obj) {
        std::printf("CGameScriptInterface_0088ec70_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL called=%d entity=%d max=%d this=%p obj=%p\n", g_called, g_entity, g_max, g_this, (void*)&obj);
    return 1;
}