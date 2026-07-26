#pragma optimize("s",on)
struct Elem {
    virtual void* scalar_dtor(int flag);
    char pad[0x4c - 4];
};

void __fastcall vector_Destroy(Elem* first, Elem* last, void* al)
{
    while (first != last) {
        first->scalar_dtor(0);
        first = (Elem*)((char*)first + 0x4c);
    }
}