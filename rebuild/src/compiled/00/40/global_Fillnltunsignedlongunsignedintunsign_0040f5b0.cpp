struct ULSlot {
    void Assign(unsigned long val);
};

unsigned long* __fastcall Fill_n(unsigned long* first, unsigned int count, unsigned long val)
{
    for (; count > 0; --count, ++first)
    {
        ULSlot* p = (ULSlot*)first;
        if (p)
            p->Assign(val);
    }
    return first;
}