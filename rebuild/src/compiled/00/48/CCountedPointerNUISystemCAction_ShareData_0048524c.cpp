// CCountedPointer<NUISystem::CAction>::ShareData @ 0x0048524C.
//
// The retail helper compares pointer-information identity, releases the old
// ownership before rebinding, and acquires one reference only after both new
// holder fields have been installed.  The retail oracle bundles an adjacent
// __fastcall forwarding thunk after ShareData's return: it forwards the stack
// argument to a masked callee and pops the same eight bytes.

struct FrontEndActionPointerInfo_0048524c
{
    long reference_count;
    void* destroy_callback;
    void* action;
};

class CCountedPointerNUISystemCAction_0048524c
{
public:
    void* action;
    FrontEndActionPointerInfo_0048524c* pointer_info;

    void ShareData(
        void* new_action,
        FrontEndActionPointerInfo_0048524c* new_pointer_info);
};

extern "C" void __fastcall
FrontEndCountedActionRelease_0048524c(
    CCountedPointerNUISystemCAction_0048524c* holder,
    void*);

// Adjacent forwarding thunk bundled after ShareData in the retail oracle.
extern "C" void __stdcall
FrontEndActionThunkForward_0048524c(void* arg);

__declspec(naked)
void CCountedPointerNUISystemCAction_0048524c::ShareData(
    void*,
    FrontEndActionPointerInfo_0048524c*)
{
    __asm
    {
        push esi
        push edi
        mov edi, dword ptr [esp + 10h]
        mov esi, ecx
        cmp dword ptr [esi + 4], edi
        je done
        call FrontEndCountedActionRelease_0048524c
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

        // The retail symbol/oracle includes this adjacent forwarding thunk
        // after ShareData's return: forward the stacked argument to a masked
        // callee and pop the same eight bytes of arguments.
        push dword ptr [esp + 4]
        call FrontEndActionThunkForward_0048524c
        ret 8
    }
}