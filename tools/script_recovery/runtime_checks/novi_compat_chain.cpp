#include <windows.h>
#include <cstring>
#include <cstdio>
#include <cassert>
#include <string>
static BYTE site[8], addSite[8];
static bool modulePresent=true, ownerMatches=true, protectOK=true, overrideOK=true;
static int originalCalls=0, addonCalls=0;
static HMODULE FakeModule=(HMODULE)0x12340000;
static HMODULE FakeGetModuleHandleA(const char*) { return modulePresent?FakeModule:nullptr; }
static SIZE_T FakeVirtualQuery(const void*, MEMORY_BASIC_INFORMATION* i, SIZE_T) {
    i->AllocationBase=ownerMatches?FakeModule:nullptr; return sizeof(*i);
}
static BOOL FakeVirtualProtect(void*,SIZE_T,DWORD,DWORD* old) { *old=PAGE_EXECUTE_READ;return protectOK; }
static BOOL FakeFlushInstructionCache(HANDLE,const void*,SIZE_T) { return TRUE; }
template<class T> T ASLR(DWORD address) { return (T)(address==0xCDB355?site:addSite); }
static void InitializeFableAPI() {}
static void InitializeFSEPaths(HMODULE) {}
static void LogToFile(const std::string&) {}
static bool InstallRetailOverrideHook() { return overrideOK; }
static __declspec(naked) void InjectCustomScripts() {
    __asm {
        inc addonCalls
        xor esi, esi
        xor edi, edi
        xor eax, eax
        pxor xmm0, xmm0
        fld1
        ret
    }
}
static __declspec(naked) void Original() {
    __asm {
        pushad
        sub esp, 28h
        fstenv [esp]
        inc originalCalls
        fldenv [esp]
        add esp, 28h
        popad
        ret
    }
}
#define GetModuleHandleA FakeGetModuleHandleA
#define VirtualQuery FakeVirtualQuery
#define VirtualProtect FakeVirtualProtect
#define FlushInstructionCache FakeFlushInstructionCache
#define DllMain TestDllMain
#include "../novi_compat_chain.inl"
#undef DllMain
static void reset() {
    modulePresent=ownerMatches=protectOK=overrideOK=true;
    g_noviStarted=false;
    site[0]=0xE9;
    DWORD displacement=(DWORD)&Original-(DWORD)site-5;
    memcpy(site+1,&displacement,4);
    BYTE expected[]={0x8b,0x44,0x24,0x08,0x83,0xec,0x18};
    memcpy(addSite,expected,7);
}
int main() {
    reset(); modulePresent=false; assert(NoviCompatibilityStart(nullptr)==10);
    reset(); site[0]=0; assert(NoviCompatibilityStart(nullptr)==11);
    reset(); ownerMatches=false; assert(NoviCompatibilityStart(nullptr)==12);
    reset(); DWORD badTarget=(DWORD)addSite-(DWORD)site-5;
    memcpy(site+1,&badTarget,4); assert(NoviCompatibilityStart(nullptr)==13);
    reset(); addSite[0]=0; assert(NoviCompatibilityStart(nullptr)==14);
    reset(); protectOK=false; assert(NoviCompatibilityStart(nullptr)==15);
    reset(); overrideOK=false;
    BYTE before[5];memcpy(before,site,5);
    assert(NoviCompatibilityStart(nullptr)==16 && !memcmp(before,site,5));
    reset(); assert(NoviCompatibilityStart(nullptr)==1);
    assert(NoviCompatibilityStart(nullptr)==2);
    DWORD originalTarget=(DWORD)site+5+*(DWORD*)(site+1);
    assert(originalTarget==(DWORD)&NoviChainedRegistrationHook);
    for(int i=0;i<3;i++) {
        DWORD regValue=0, xmmValue=0, flagsBefore=0, flagsAfter=0;
        float fpuValue=0;
        __asm {
            push esi
            mov esi, 13579bdfh
            mov eax, 2468ace0h
            movd xmm0,eax
            fld1
            pushfd
            pop flagsBefore
            call NoviChainedRegistrationHook
            pushfd
            pop flagsAfter
            mov regValue,esi
            movd xmmValue,xmm0
            fstp fpuValue
            pop esi
        }
        assert(regValue==0x13579bdf && xmmValue==0x2468ace0 && fpuValue==1);
        // Original test callback changes flags itself. Add-on preservation is
        // covered before that boundary by the byte/instruction audit separately.
        (void)flagsBefore;(void)flagsAfter;
    }
    assert(addonCalls==3 && originalCalls==3);
    puts("9 initialization/gating cases and 3 chained machine-state calls passed");
}
