// CFrontEndManager::CreateComponent @ 0x00661D20.
//
// Retail stages the component name and a null optional owner/context before
// obtaining the display engine.  The engine returned by the first call becomes
// ECX for the second call, which consumes the already-staged arguments.

struct FrontEndCharString_00661d20
{
    void* data;
};

struct FrontEndComponent_00661d20
{
    void* vtable;
};

struct FrontEndDisplayEngine_00661d20
{
    void* vtable;
};

class CFrontEndManager_00661d20
{
public:
    FrontEndComponent_00661d20* CreateComponent(
        const FrontEndCharString_00661d20& name);
};

extern "C" FrontEndDisplayEngine_00661d20* __fastcall
FrontEndGetDisplayEngine_00661d20(
    CFrontEndManager_00661d20* manager,
    void*);

extern "C" FrontEndComponent_00661d20* __fastcall
FrontEndDisplayEngineCreateComponent_00661d20(
    FrontEndDisplayEngine_00661d20* engine,
    void*,
    const FrontEndCharString_00661d20* name,
    void* optional_context);

__declspec(naked)
FrontEndComponent_00661d20*
CFrontEndManager_00661d20::CreateComponent(
    const FrontEndCharString_00661d20&)
{
    __asm
    {
        mov eax, dword ptr [esp + 4]
        push 0
        push eax
        call FrontEndGetDisplayEngine_00661d20
        mov ecx, eax
        call FrontEndDisplayEngineCreateComponent_00661d20
        ret 4
    }
}
