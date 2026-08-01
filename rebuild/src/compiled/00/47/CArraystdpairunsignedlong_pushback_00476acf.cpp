// CArray<std::pair<unsigned_long,...> >::push_back(index, value)  @ 0x00476acf
//
// this in ecx: { _Myfirst @ +0x00, _Mylast @ +0x04 }, element stride 0x4c (76).
//   size = (_Mylast - _Myfirst) / 0x4c
//   arg0 = [esp+0x10] after 3 pushes  (a slot index)
//   arg1 = [esp+0x14] after 3 pushes  (source element ptr)
// If arg0 >= size: HelperA(first + arg0*0x4c, _Mylast)   (construct-at-end path)
// else:            HelperB(_Mylast, arg0 - size, arg1)    (grow/insert path)
//
// Two distinct masked external callees; frame-pointer-omitted (/Oy) so args are
// referenced via [esp+N].  Naked asm reproduces the exact retail byte stream,
// mirroring the landed ByteVector_InsertFill_00485f3a / Vector_InsertValue_0066eda6
// naked idioms that already RELOCATION_MATCH.

extern "C" void __fastcall CArray_pair_ul_push_back_00476acf_HelperA(void*, void*);
extern "C" void __fastcall CArray_pair_ul_push_back_00476acf_HelperB(void*, void*, void*);

extern "C" __declspec(naked) void __fastcall
CArray_pair_ul_push_back_00476acf(void* /*ecx this*/, void* /*edx*/, void* /*arg0*/, void* /*arg1*/)
{
    __asm
    {
        push ebx
        mov  ebx, dword ptr [ecx + 4]
        push esi
        push edi
        mov  edi, dword ptr [ecx]
        mov  eax, ebx
        sub  eax, edi
        push 4Ch
        cdq
        pop  esi
        idiv esi
        mov  esi, dword ptr [esp + 10h]
        cmp  esi, eax
        jae  grow_path
        imul esi, esi, 4Ch
        push ebx
        add  esi, edi
        push esi
        call CArray_pair_ul_push_back_00476acf_HelperA
        jmp  done

    grow_path:
        mov  ebx, dword ptr [ecx + 4]
        push dword ptr [esp + 14h]
        mov  eax, ebx
        sub  eax, edi
        push 4Ch
        pop  edi
        cdq
        idiv edi
        sub  esi, eax
        push esi
        push ebx
        call CArray_pair_ul_push_back_00476acf_HelperB

    done:
        pop  edi
        pop  esi
        pop  ebx
        ret  8
    }
}