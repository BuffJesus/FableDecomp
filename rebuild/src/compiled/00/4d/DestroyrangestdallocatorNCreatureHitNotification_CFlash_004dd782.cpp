// _Destroy_range<allocator<NCreatureHitNotification::CFlash> >
// retail 0x004dd782 : __fastcall(first, last, allocator*, tag), stride 8, ret 4
//
// The loop walks [first,last) stride 8, invoking each element's scalar-deleting
// destructor (a __fastcall taking one int arg = 0, "don't free") out-of-line at
// retail 0x4da68a. Retail codegen:
//   push esi ; push edi ; mov edi,edx ; mov esi,ecx ; jmp cmp
//   body: push 0 ; mov ecx,esi ; call dtor ; add esi,8
//   cmp:  cmp esi,edi ; jne body
//   pop edi ; pop esi ; ret 4
//
// Modelled with a declared-only member so codegen is a single external call
// (relocation) per element and no dtor body is emitted here.

struct CFlash
{
    unsigned long a;
    void* p;
    void DestroyElement(int mode);   // __fastcall, defined elsewhere (0x4da68a)
};

struct allocator_flash {};

#pragma optimize("s", on)
void __fastcall Destroy_range_CFlash(CFlash* first, CFlash* last,
                                     allocator_flash* al)
{
    (void)al;
    while (first != last)
    {
        first->DestroyElement(0);
        ++first;
    }
}
#pragma optimize("", on)