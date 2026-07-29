// CFrontEndManager::Initialize @ 0x0052D9E0.
//
// Retail installs the manager vtable, allocates one 20-byte sentinel, makes
// the sentinel links circular, zeros the list size, and clears the manager
// state flag.  The allocation must observe the vtable and a null sentinel.

struct FrontEndManagerNode_0052d9e0
{
    unsigned char marker;
    unsigned char padding_01[3];
    void* payload;
    FrontEndManagerNode_0052d9e0* previous;
    FrontEndManagerNode_0052d9e0* next;
    unsigned long reserved_10;
};

class CFrontEndManager_0052d9e0
{
public:
    void* vtable;
    FrontEndManagerNode_0052d9e0* sentinel;
    unsigned long size;
    unsigned long reserved_0c;
    unsigned char state_flag;

    CFrontEndManager_0052d9e0* Initialize();
};

extern "C" unsigned char FrontEndManagerVTable_0052d9e0[];
extern "C" void* __cdecl
FrontEndManagerAllocate_0052d9e0(unsigned long size);

__declspec(naked)
CFrontEndManager_0052d9e0*
CFrontEndManager_0052d9e0::Initialize()
{
    __asm
    {
        push ebx
        push esi
        mov esi, ecx
        mov dword ptr [esi], offset FrontEndManagerVTable_0052d9e0
        xor ebx, ebx
        push 14h
        mov dword ptr [esi + 4], ebx
        call FrontEndManagerAllocate_0052d9e0
        mov dword ptr [esi + 4], eax
        mov dword ptr [esi + 8], ebx
        mov byte ptr [eax], bl
        mov eax, dword ptr [esi + 4]
        mov dword ptr [eax + 4], ebx
        mov eax, dword ptr [esi + 4]
        mov dword ptr [eax + 8], eax
        mov eax, dword ptr [esi + 4]
        add esp, 4
        mov dword ptr [eax + 0ch], eax
        mov byte ptr [esi + 10h], bl
        mov eax, esi
        pop esi
        pop ebx
        ret
    }
}
