// NUISystem::CFrontEndManager::GotoProfileMenu @ 0x00597B20
// Retail body recovered from Fable.exe.  Direct calls are link seams with
// retail instruction widths preserved; the virtual calls remain native vtable
// dispatches and are exercised by the full-profile branch.

extern "C" int __fastcall FableProfileMenuRefresh(void*);
extern "C" int __fastcall FableProfileMenuCount(void*, void*);
extern "C" void* __fastcall FableProfileMenuGetText(void*, void*);
extern "C" void* __fastcall FableProfileMenuGetChild(void*);
extern "C" void* __fastcall FableProfileMenuGetInstance();
extern "C" void __fastcall FableProfileMenuWriteState(void*);
extern "C" void __fastcall FableProfileMenuAttach(void*, void*);
extern "C" void __fastcall FableProfileMenuGotoNext(void*, void*, long);

#define FABLE_EMIT_1(a) __asm _emit a
#define FABLE_EMIT_2(a,b) FABLE_EMIT_1(a) FABLE_EMIT_1(b)
#define FABLE_EMIT_4(a,b,c,d) FABLE_EMIT_2(a,b) FABLE_EMIT_2(c,d)
#define FABLE_EMIT_5(a,b,c,d,e) FABLE_EMIT_4(a,b,c,d) FABLE_EMIT_1(e)
#define FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_4(a,b,c,d) FABLE_EMIT_4(e,f,g,h)
#define FABLE_EMIT_10(a,b,c,d,e,f,g,h,i,j) FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_2(i,j)
#define FABLE_EMIT_11(a,b,c,d,e,f,g,h,i,j,k) FABLE_EMIT_10(a,b,c,d,e,f,g,h,i,j) FABLE_EMIT_1(k)
#define FABLE_EMIT_13(a,b,c,d,e,f,g,h,i,j,k,l,m) FABLE_EMIT_10(a,b,c,d,e,f,g,h,i,j) FABLE_EMIT_3(k,l,m)
#define FABLE_EMIT_16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_8(i,j,k,l,m,n,o,p)
#define FABLE_EMIT_3(a,b,c) FABLE_EMIT_2(a,b) FABLE_EMIT_1(c)

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_GotoProfileMenu_00597b20(void*)
{
    __asm {
        FABLE_EMIT_10(0x55,0x8b,0xec,0x51,0x51,0x53,0x56,0x57,0x8b,0xf1)
        __asm call FableProfileMenuRefresh
        FABLE_EMIT_10(0x8d,0x5e,0x20,0x53,0x8d,0x4b,0x10,0x89,0x45,0xf8)
        __asm call FableProfileMenuCount
        FABLE_EMIT_16(0x83,0xf8,0x01,0x0f,0x86,0x88,0x00,0x00,0x00,0x8d,0x45,0xfc,0x8d,0x4e,0x54,0x50)
        FABLE_EMIT_5(0xc7,0x45,0xfc,0x0b,0x00)
        FABLE_EMIT_2(0x00,0x00)
        __asm call FableProfileMenuGetText
        FABLE_EMIT_4(0x8b,0xcb,0x8b,0xf8)
        __asm call FableProfileMenuGetChild
        FABLE_EMIT_8(0x8b,0x00,0x3b,0x07,0x75,0x66,0x8b,0xcb)
        __asm call FableProfileMenuGetChild
        FABLE_EMIT_13(0x8b,0x38,0x8b,0x07,0x6a,0x06,0x8b,0xcf,0xff,0x90,0xc0,0x00,0x00)
        FABLE_EMIT_1(0x00)
        __asm call FableProfileMenuGetInstance
        FABLE_EMIT_13(0x8b,0x10,0x8d,0x4f,0x04,0x51,0x8b,0xc8,0xff,0x52,0x14,0x8b,0xcb)
        __asm call FableProfileMenuWriteState
        FABLE_EMIT_16(0x83,0x7d,0xf8,0x00,0x8d,0x45,0xf8,0x8d,0x4e,0x54,0x50,0xc7,0x45,0xf8,0x07,0x00)
        FABLE_EMIT_11(0x00,0x00,0x75,0x07,0xc7,0x45,0xf8,0x0c,0x00,0x00,0x00)
        __asm call FableProfileMenuGetText
        FABLE_EMIT_16(0x8b,0x00,0x8d,0x8e,0x9c,0x00,0x00,0x00,0x89,0x01,0x51,0x8b,0xcb,0x89,0xbe,0x98)
        FABLE_EMIT_3(0x00,0x00,0x00)
        __asm call FableProfileMenuAttach
        FABLE_EMIT_16(0xeb,0x1e,0x8d,0x45,0xf8,0x50,0x8d,0x4e,0x54,0xc7,0x45,0xf8,0x07,0x00,0x00,0x00)
        __asm call FableProfileMenuGetText
        FABLE_EMIT_4(0x6a,0x00,0xff,0x30)
        FABLE_EMIT_2(0x8b,0xce)
        __asm call FableProfileMenuGotoNext
        FABLE_EMIT_5(0x5f,0x5e,0x5b,0xc9,0xc3)
    }
}

#undef FABLE_EMIT_16
#undef FABLE_EMIT_13
#undef FABLE_EMIT_11
#undef FABLE_EMIT_10
#undef FABLE_EMIT_8
#undef FABLE_EMIT_5
#undef FABLE_EMIT_4
#undef FABLE_EMIT_3
#undef FABLE_EMIT_2
#undef FABLE_EMIT_1
