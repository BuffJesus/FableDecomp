// Front-end CCountedPointer<NUISystem::CAction> release helper @ 0x0059A994.
//
// The stale catalog prototype labels ECX as an allocator for an unrelated
// QuadricOptimiser counted pointer.  All three retail callers instead pass an
// eight-byte front-end action holder in ECX: the action at +0 and its shared
// pointer information at +4.

struct FrontEndActionPointerInfo_0059a994
{
    long reference_count;
    void* destroy_callback;
    void* action;
};

class FrontEndCountedActionPointer_0059a994
{
public:
    void* action;
    FrontEndActionPointerInfo_0059a994* pointer_info;

    void Release();
};

extern "C" void __cdecl
FrontEndCountedPointerDelete_0059a994(void* allocation);

__declspec(naked)
void FrontEndCountedActionPointer_0059a994::Release()
{
    __asm
    {
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi + 4]
        test eax, eax
        je clear_holder
        dec dword ptr [eax]
        mov eax, dword ptr [esi + 4]
        cmp dword ptr [eax], 0
        jne clear_holder
        mov ecx, dword ptr [eax + 8]
        call dword ptr [eax + 4]
        push dword ptr [esi + 4]
        call FrontEndCountedPointerDelete_0059a994
        pop ecx

    clear_holder:
        and dword ptr [esi], 0
        and dword ptr [esi + 4], 0
        pop esi
        ret
    }
}
