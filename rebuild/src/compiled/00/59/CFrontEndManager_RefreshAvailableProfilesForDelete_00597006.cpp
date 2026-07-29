// NUISystem::CFrontEndManager::RefreshAvailableProfilesForDelete @ 0x00597006
//
// Address-isolated VC7.1 reconstruction of the delete-profile list rebuild.

struct CWideString_00597006 { void* data; };
struct CCharString_00597006 { const char* data; };
struct CComponent_00597006 { void** vtable; };
struct CCountedComponent_00597006
{
    CComponent_00597006* object;
    void* pointerInfo;
};
struct CProfileVector_00597006
{
    CWideString_00597006* begin;
    CWideString_00597006* end;
    CWideString_00597006* capacity;
};
struct CUsedComponentMap_00597006 { char data[0x0c]; };
struct CAction_00597006 { unsigned int type; char data[0x0c]; };
struct CActionParamString_00597006 { char data[0x0c]; };

class CFrontEndManager_00597006
{
public:
    char pad000[0x54];
    CUsedComponentMap_00597006 usedComponents;

    long RefreshAvailableProfilesForDelete();
};

extern "C" const char FrontEndDeleteListDefinition_00597006[];
extern "C" const char FrontEndEmptyDefinition_00597006[];
extern "C" const char FrontEndDeleteButtonDefinition_00597006[];
extern "C" const float FrontEndProfileSpacing_00597006;

extern "C" void __fastcall FrontEndGetProfileNames_00597006(
    CFrontEndManager_00597006*, void*, CProfileVector_00597006*);
extern "C" void* __cdecl FrontEndComponentManagerGet_00597006();
extern "C" CComponent_00597006** __fastcall FrontEndUsedLookup_00597006(
    CUsedComponentMap_00597006*, void*, const int*);
extern "C" void __fastcall FrontEndCharConstruct_00597006(
    CCharString_00597006*, void*, const char*, int);
extern "C" void __fastcall FrontEndTemporaryListInit_00597006(void*, void*);
extern "C" void __fastcall FrontEndTemporaryPairConstruct_00597006(
    void*, void*, const void*);
extern "C" void __fastcall FrontEndTemporaryListDestroy_00597006(void*, void*);
extern "C" CComponent_00597006* __fastcall
FrontEndManagerCreateComponent_00597006(
    void*, void*, const CCharString_00597006*, bool);
extern "C" void __fastcall FrontEndCountedConstruct_00597006(
    CCountedComponent_00597006*, void*, CComponent_00597006*);
extern "C" void __fastcall FrontEndCharDestroy_00597006(
    CCharString_00597006*, void*);
extern "C" void __fastcall FrontEndCountedShare_00597006(
    CCountedComponent_00597006*, void*, CComponent_00597006*, void*);
extern "C" CComponent_00597006* __fastcall
FrontEndCreateDeleteButton_00597006(
    CFrontEndManager_00597006*,
    void*,
    CWideString_00597006*,
    float,
    const CCharString_00597006*,
    const CCharString_00597006*,
    bool);
extern "C" void* __cdecl FrontEndAllocate_00597006(unsigned int);
extern "C" CAction_00597006* __fastcall FrontEndActionConstruct_00597006(
    CAction_00597006*, void*);
extern "C" void __fastcall FrontEndActionCountedConstruct_00597006(
    CCountedComponent_00597006*, void*, CComponent_00597006*);
extern "C" CActionParamString_00597006* __fastcall
FrontEndActionParamConstruct_00597006(
    CActionParamString_00597006*, void*, CWideString_00597006*);
extern "C" void __fastcall FrontEndActionParamAssign_00597006(
    void*, void*, CActionParamString_00597006**);
extern "C" void __fastcall FrontEndActionRelease_00597006(
    CCountedComponent_00597006*, void*);
extern "C" void __fastcall FrontEndComponentRelease_00597006(
    CCountedComponent_00597006*, void*);
extern "C" void __fastcall FrontEndListRelease_00597006(
    CCountedComponent_00597006*, void*);
extern "C" void __fastcall FrontEndProfileVectorTidy_00597006(
    CProfileVector_00597006*, void*);

__declspec(naked)
long CFrontEndManager_00597006::RefreshAvailableProfilesForDelete()
{
    __asm
    {
        push ebp
        mov ebp, esp
        sub esp, 48h
        push ebx
        xor eax, eax
        push esi
        mov dword ptr [ebp - 48h], eax
        mov dword ptr [ebp - 44h], eax
        mov dword ptr [ebp - 40h], eax
        push edi
        lea eax, [ebp - 48h]
        mov esi, ecx
        push eax
        mov dword ptr [ebp - 1ch], esi
        call FrontEndGetProfileNames_00597006

        call FrontEndComponentManagerGet_00597006
        mov dword ptr [ebp - 14h], eax
        lea eax, [ebp - 18h]
        push eax
        lea ecx, [esi + 54h]
        mov dword ptr [ebp - 18h], 15h
        call FrontEndUsedLookup_00597006
        mov esi, dword ptr [eax]
        mov ebx, dword ptr [esi]

        push ecx
        mov ecx, esp
        push -1
        mov edi, offset FrontEndDeleteListDefinition_00597006
        push edi
        call FrontEndCharConstruct_00597006
        mov ecx, esi
        call dword ptr [ebx + 0ch]
        mov ebx, eax
        test ebx, ebx
        mov dword ptr [ebp - 18h], ebx
        je createReplacementList

        lea ecx, [ebp - 34h]
        call FrontEndTemporaryListInit_00597006
        mov ebx, dword ptr [ebx]
        push ecx
        push ecx
        lea eax, [ebp - 34h]
        mov ecx, esp
        push eax
        mov dword ptr [ebp - 34h], 1
        call FrontEndTemporaryPairConstruct_00597006
        mov ecx, dword ptr [ebp - 18h]
        call dword ptr [ebx + 0f8h]
        lea ecx, [ebp - 30h]
        call FrontEndTemporaryListDestroy_00597006

createReplacementList:
        push -1
        push edi
        lea ecx, [ebp - 8]
        call FrontEndCharConstruct_00597006
        mov ecx, dword ptr [ebp - 14h]
        xor edi, edi
        push edi
        lea eax, [ebp - 8]
        push eax
        call FrontEndManagerCreateComponent_00597006
        push eax
        lea ecx, [ebp - 3ch]
        call FrontEndCountedConstruct_00597006
        lea ecx, [ebp - 8]
        call FrontEndCharDestroy_00597006

        push dword ptr [ebp - 38h]
        lea ecx, [ebp - 34h]
        push dword ptr [ebp - 3ch]
        mov dword ptr [ebp - 34h], edi
        mov dword ptr [ebp - 30h], edi
        call FrontEndCountedShare_00597006

        mov eax, dword ptr [esi]
        lea ecx, [ebp - 3ch]
        push ecx
        mov ecx, esi
        call dword ptr [eax + 0ech]

        fldz
        mov edi, dword ptr [ebp - 44h]
        fstp dword ptr [ebp - 8]
        mov eax, dword ptr [ebp - 48h]
        sub edi, eax
        sar edi, 2
        je finishProfiles

        mov ebx, dword ptr [ebp - 34h]
        mov dword ptr [ebp - 4], eax
        mov dword ptr [ebp - 18h], edi

profileLoop:
        push -1
        push offset FrontEndEmptyDefinition_00597006
        lea ecx, [ebp - 10h]
        call FrontEndCharConstruct_00597006
        push -1
        push offset FrontEndDeleteButtonDefinition_00597006
        lea ecx, [ebp - 0ch]
        call FrontEndCharConstruct_00597006

        fld dword ptr [ebp - 8]
        push 0
        lea eax, [ebp - 10h]
        push eax
        lea eax, [ebp - 0ch]
        push eax
        push ecx
        mov ecx, dword ptr [ebp - 1ch]
        fstp dword ptr [esp]
        push dword ptr [ebp - 4]
        call FrontEndCreateDeleteButton_00597006
        push eax
        lea ecx, [ebp - 2ch]
        call FrontEndCountedConstruct_00597006
        lea ecx, [ebp - 0ch]
        call FrontEndCharDestroy_00597006
        lea ecx, [ebp - 10h]
        call FrontEndCharDestroy_00597006

        push 10h
        call FrontEndAllocate_00597006
        test eax, eax
        pop ecx
        je noAction
        mov ecx, eax
        call FrontEndActionConstruct_00597006
        jmp haveAction
noAction:
        xor eax, eax
haveAction:
        push eax
        lea ecx, [ebp - 24h]
        call FrontEndActionCountedConstruct_00597006

        push 0ch
        call FrontEndAllocate_00597006
        test eax, eax
        pop ecx
        je noParameter
        push dword ptr [ebp - 4]
        mov ecx, eax
        call FrontEndActionParamConstruct_00597006
        mov dword ptr [ebp - 14h], eax
        jmp haveParameter
noParameter:
        and dword ptr [ebp - 14h], 0
haveParameter:
        mov eax, dword ptr [ebp - 24h]
        mov dword ptr [eax], 0d7h
        mov ecx, dword ptr [ebp - 24h]
        lea eax, [ebp - 14h]
        push eax
        add ecx, 4
        call FrontEndActionParamAssign_00597006

        mov ecx, dword ptr [ebp - 2ch]
        mov eax, dword ptr [ecx]
        lea edx, [ebp - 24h]
        push edx
        call dword ptr [eax + 108h]
        mov ecx, dword ptr [ebp - 2ch]
        mov eax, dword ptr [ecx]
        lea edx, [ebp - 24h]
        push edx
        call dword ptr [eax + 11ch]

        mov eax, dword ptr [ebx]
        lea ecx, [ebp - 2ch]
        push ecx
        mov ecx, ebx
        call dword ptr [eax + 0f0h]

        fld dword ptr [ebp - 8]
        fadd dword ptr [FrontEndProfileSpacing_00597006]
        lea ecx, [ebp - 24h]
        fstp dword ptr [ebp - 8]
        call FrontEndActionRelease_00597006
        lea ecx, [ebp - 2ch]
        call FrontEndComponentRelease_00597006

        add dword ptr [ebp - 4], 4
        dec dword ptr [ebp - 18h]
        jne profileLoop

finishProfiles:
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 0ach]
        lea ecx, [ebp - 34h]
        call FrontEndListRelease_00597006
        lea ecx, [ebp - 3ch]
        call FrontEndComponentRelease_00597006
        lea ecx, [ebp - 48h]
        call FrontEndProfileVectorTidy_00597006

        mov eax, edi
        pop edi
        pop esi
        pop ebx
        leave
        ret
    }
}
