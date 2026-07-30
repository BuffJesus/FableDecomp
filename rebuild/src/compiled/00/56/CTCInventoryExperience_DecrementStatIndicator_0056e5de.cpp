// CTCInventoryExperience::DecrementStatIndicator @ 0x0056e5de
// this=ecx. Decrement counter at this+0x1bc; clamp so it never goes negative.
struct CTCInventoryExperience {
    char pad[0x1bc];
    int count;
    int* __fastcall DecrementStatIndicator();
};

int* __fastcall CTCInventoryExperience::DecrementStatIndicator()
{
    int* p = &this->count;
    int v = *p - 1;
    if (v >= 0)
        *p = v;
    return p;
}