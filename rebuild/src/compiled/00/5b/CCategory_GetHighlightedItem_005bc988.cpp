// NInventory::CCategory::GetHighlightedItem @ retail 0x005bc988 (20 bytes).
//
// Retail disassembly (relocation-masked parity):
//   0: 8b 41 24    mov eax,DWORD PTR [ecx+0x24]   ; signed highlighted index
//   3: 85 c0       test eax,eax
//   5: 7c 0a       jl  0x11                       ; index < 0 -> return 0
//   7: 8b 49 08    mov ecx,DWORD PTR [ecx+0x8]    ; item array base
//   a: 8d 04 80    lea eax,[eax+eax*4]            ; index * 5
//   d: 8b 04 81    mov eax,DWORD PTR [ecx+eax*4]  ; *(u32*)(base + index*20)
//  10: c3          ret
//  11: 33 c0       xor eax,eax
//  13: c3          ret
//
// Member accessor modelled as __fastcall(this in ecx). VC7.1 / no C++11.

typedef signed int fable_i32;
typedef unsigned int fable_u32;

// One item slot is 20 bytes (0x14 stride from lea*5 + scale*4).
struct CCategory_Item_005bc988
{
    fable_u32 Handle00;      // +0x00: first dword returned by the accessor
    fable_u32 Pad04;         // +0x04
    fable_u32 Pad08;         // +0x08
    fable_u32 Pad0C;         // +0x0C
    fable_u32 Pad10;         // +0x10
};

struct CCategory_005bc988
{
    fable_u32 Unknown00;                 // +0x00
    fable_u32 Unknown04;                 // +0x04
    CCategory_Item_005bc988* Items08;    // +0x08: base of the item array
    fable_u32 Unknown0C;                 // +0x0C
    fable_u32 Unknown10;                 // +0x10
    fable_u32 Unknown14;                 // +0x14
    fable_u32 Unknown18;                 // +0x18
    fable_u32 Unknown1C;                 // +0x1C
    fable_u32 Unknown20;                 // +0x20
    fable_i32 Highlighted24;             // +0x24: signed highlighted index
};

typedef char fable_offset_check_08[((int)&((CCategory_005bc988*)0)->Items08 == 0x08) ? 1 : -1];
typedef char fable_offset_check_24[((int)&((CCategory_005bc988*)0)->Highlighted24 == 0x24) ? 1 : -1];
typedef char fable_stride_check[(sizeof(CCategory_Item_005bc988) == 0x14) ? 1 : -1];

fable_u32 __fastcall CCategory_GetHighlightedItem_005bc988(const CCategory_005bc988* self)
{
    fable_i32 index = self->Highlighted24;
    if (index >= 0)
    {
        return self->Items08[index].Handle00;
    }
    return 0;
}
