struct PointerSlot { void* value; };
extern "C" unsigned long __stdcall Getter_TripleDeref(PointerSlot* input)
{
    PointerSlot* level1 = (PointerSlot*)input->value;
    PointerSlot* level2 = (PointerSlot*)level1->value;
    unsigned long* level3 = (unsigned long*)level2->value;
    return *level3;
}