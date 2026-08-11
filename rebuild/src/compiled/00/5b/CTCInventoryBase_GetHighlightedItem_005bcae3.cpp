#pragma optimize("s",on)
// CTCInventoryBase::GetHighlightedItem  (retail 0x005bcae3)
//
// mov eax, ecx
// mov ecx, [eax+0x20]        ; begin ptr
// cmp ecx, [eax+0x24]        ; end ptr
// je  ret0
// mov ecx, [eax+0x70]        ; highlight index
// imul ecx, ecx, 0x2c        ; * sizeof(element)=44
// add ecx, [eax+0x20]        ; element = begin + idx*44
// jmp element->GetHandle()   ; tail call, __fastcall on element*
// ret0: xor eax,eax ; ret

struct InvItem {                 // 44-byte element (0x2c)
    char data[0x2c];
    long GetHandle();            // __fastcall on element ptr
};

struct InvVec {
    InvItem* first;   // +0x00 (this+0x20)
    InvItem* last;    // +0x04 (this+0x24)
    bool empty() const { return first == last; }
};

struct CTCInventoryBase {
    char pad00[0x20];
    InvVec items;     // +0x20 { first@0x20, last@0x24 }
    char pad28[0x70 - 0x28];
    int   highlight;  // +0x70

    long GetHighlightedItem();
};

long CTCInventoryBase::GetHighlightedItem()
{
    if (!this->items.empty())
        return this->items.first[this->highlight].GetHandle();
    return 0;
}