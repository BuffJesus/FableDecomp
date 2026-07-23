struct CharStringStorage
{
    const void* data;
    unsigned long length;
};

struct CCharStringOverlay
{
    CharStringStorage* storage;
};

class CCRC
{
public:
    static unsigned long __fastcall Calc(
        unsigned long seed, const void* data, unsigned long length);
};

extern unsigned char DAT_0129aaf4;

__declspec(naked) unsigned long __fastcall CCharString_ComputeCRC32(CCharStringOverlay*)
{
    __asm
    {
        mov eax, dword ptr [ecx]
        test eax, eax
        jne storage_present
        xor ecx, ecx
        jmp length_ready
storage_present:
        mov ecx, dword ptr [eax+4]
length_ready:
        test eax, eax
        jne data_present
        mov eax, offset DAT_0129aaf4
        push ecx
        mov edx, eax
        xor ecx, ecx
        call CCRC::Calc
        ret
data_present:
        mov eax, dword ptr [eax]
        push ecx
        mov edx, eax
        xor ecx, ecx
        call CCRC::Calc
        ret
    }
}
