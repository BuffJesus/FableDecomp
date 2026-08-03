// GetAnimGroupName(int index):
//   mov eax, ds:[g_AnimGroupNameTable]  ; load global base pointer
//   lea eax, [eax + ecx*4]              ; index by ecx (fastcall arg), 4-byte stride
//   ret
// Returns &g_AnimGroupNameTable[index] -- a pointer into a global table of 4-byte entries.
extern void** g_AnimGroupNameTable;

__declspec(noinline) void** __fastcall GetAnimGroupName(int index)
{
    return g_AnimGroupNameTable + index;
}