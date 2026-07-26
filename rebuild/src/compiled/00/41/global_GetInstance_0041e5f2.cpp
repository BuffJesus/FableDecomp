#pragma optimize("s",on)
// NUISystem::CFrontEndManager::GetInstance @ 0x0041e5f2  (VC7.1 / MSVC 2003)
// Lazy singleton accessor. Compile: cl /c /O2 /Oy /W3
//
// Retail shape:
//   mov  eax, ds:g_instance     ; load static
//   test eax,eax / jne ret      ; already created -> return it (no re-store)
//   push 0xd0 / call new        ; operator new(208)
//   test eax,eax / pop ecx / je null
//   mov  ecx,eax / call ctor    ; construct, ctor returns this
//   jmp store
// null: xor eax,eax
// store: mov ds:g_instance, eax ; ret

// ---- operator new (relocation-masked call target) ----
void * operator new(unsigned int size);

// ---- CFrontEndManager: opaque, exactly 208 (0xd0) bytes ----
struct CFrontEndManager
{
    unsigned char _pad[0xd0];
};

// The engine constructor: __fastcall so 'this' arrives in ecx (byte-identical to
// thiscall) and it returns 'this' in eax (matches the stored value). Body is in
// the engine; declared extern, call is relocation-masked.
extern "C" CFrontEndManager * __fastcall CFrontEndManager_ctor(CFrontEndManager *self);

// The single static instance pointer (abs-addr reads/writes relocation-masked).
static CFrontEndManager * g_instance = 0;

// Member accessor modeled as a free __fastcall(void): no params, this-less.
CFrontEndManager * __fastcall NUISystem_CFrontEndManager_GetInstance(void)
{
    CFrontEndManager *p = g_instance;
    if (p == 0)
    {
        void *mem = operator new(0xd0);
        if (mem != 0)
            p = CFrontEndManager_ctor((CFrontEndManager *)mem);
        else
            p = 0;
        g_instance = p;
    }
    return p;
}