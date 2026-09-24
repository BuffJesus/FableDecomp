// NUISystem::CFrontEndManager::GetInstance @ 0x0041E5F2
// Retail singleton accessor.  The retail singleton slot, allocator call,
// and constructor call are represented as link seams for the isolated gate;
// instruction widths and control flow are the retail body.

extern "C" void* FableFrontEndManagerInstance;
extern "C" void* FableFrontEndManagerAllocate(unsigned long);
extern "C" void* __fastcall FableFrontEndManagerConstruct(void*);

#define FABLE_EMIT_1(a) __asm _emit a
#define FABLE_EMIT_2(a,b) FABLE_EMIT_1(a) FABLE_EMIT_1(b)
#define FABLE_EMIT_5(a,b,c,d,e) FABLE_EMIT_2(a,b) FABLE_EMIT_2(c,d) FABLE_EMIT_1(e)

extern "C" __declspec(naked) void*
CFrontEndManager_GetInstance_0041e5f2()
{
    __asm {
        __asm mov eax, dword ptr [FableFrontEndManagerInstance]
        FABLE_EMIT_2(0x85,0xc0)
        FABLE_EMIT_2(0x75,0x1f)
        FABLE_EMIT_5(0x68,0xd0,0x00,0x00,0x00)
        __asm call FableFrontEndManagerAllocate
        FABLE_EMIT_2(0x85,0xc0)
        FABLE_EMIT_1(0x59)
        FABLE_EMIT_2(0x74,0x09)
        FABLE_EMIT_2(0x8b,0xc8)
        __asm call FableFrontEndManagerConstruct
        FABLE_EMIT_2(0xeb,0x02)
        FABLE_EMIT_2(0x33,0xc0)
        __asm mov dword ptr [FableFrontEndManagerInstance], eax
        FABLE_EMIT_1(0xc3)
    }
}

#undef FABLE_EMIT_5
#undef FABLE_EMIT_2
#undef FABLE_EMIT_1
