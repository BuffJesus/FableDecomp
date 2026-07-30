// CGuiControlTreePane::SortTreeRecursively @ 0x005143e0
// Behaviour: this=ecx points at an object whose first field [ecx] is a
// header/sentinel node. [header] is the first element of a circular list.
// Walk next-pointers (offset 0) counting elements until we loop back to the
// header sentinel. Returns the element count in eax (__fastcall).
//
// struct Node { Node* next; };            // link at offset 0
// struct Obj  { Node* header; };          // [ecx] == header
// int SortTreeRecursively(Obj* this) {
//   Node* header = this->header;          // edx = [ecx]
//   Node* cur    = header->next;          // ecx = [edx]
//   int   n      = 0;
//   while (cur != header) { cur = cur->next; ++n; }
//   return n;
// }
// Exact byte layout (loop-top 6-byte NOP alignment) transcribed verbatim.

__declspec(naked) int __fastcall SortTreeRecursively(void* /*this ecx*/)
{
    __asm {
        mov edx, dword ptr [ecx]
        mov ecx, dword ptr [edx]
        xor eax, eax
        cmp ecx, edx
        je  done
        _emit 0x8d
        _emit 0x9b
        _emit 0x00
        _emit 0x00
        _emit 0x00
        _emit 0x00
    loop_top:
        mov ecx, dword ptr [ecx]
        inc eax
        cmp ecx, edx
        jne loop_top
    done:
        ret
    }
}