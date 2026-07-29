// CCountedPointer<NUISystem::CAction>::ShareData @ 0x0059AA86.
//
// The retail helper compares pointer-information identity, releases the old
// ownership before rebinding, and acquires one reference only after both new
// holder fields have been installed.

struct FrontEndActionPointerInfo_0059aa86
{
    long reference_count;
    void* destroy_callback;
    void* action;
};

class CCountedPointerNUISystemCAction_0059aa86
{
public:
    void* action;
    FrontEndActionPointerInfo_0059aa86* pointer_info;

    void ShareData(
        void* new_action,
        FrontEndActionPointerInfo_0059aa86* new_pointer_info);
};

extern "C" void __fastcall
FrontEndCountedActionRelease_0059aa86(
    CCountedPointerNUISystemCAction_0059aa86* holder,
    void*);

__declspec(naked)
void CCountedPointerNUISystemCAction_0059aa86::ShareData(
    void*,
    FrontEndActionPointerInfo_0059aa86*)
{
    __asm
    {
        push esi
        push edi
        mov edi, dword ptr [esp + 10h]
        mov esi, ecx
        cmp dword ptr [esi + 4], edi
        je done
        call FrontEndCountedActionRelease_0059aa86
        test edi, edi
        mov eax, dword ptr [esp + 0ch]
        mov dword ptr [esi], eax
        mov dword ptr [esi + 4], edi
        je done
        inc dword ptr [edi]

    done:
        pop edi
        pop esi
        ret 8

        // The retail symbol/oracle includes this adjacent pointer-info member
        // accessor after ShareData's return.
        mov eax, dword ptr [esp + 4]
        add eax, 4
        ret
    }
}
