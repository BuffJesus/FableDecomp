#pragma optimize("s",on)
struct Elem { virtual void dtor(int); };

void __fastcall vec_Destroy(Elem* first, Elem* last, void* al)
{
    for (; first != last; first = (Elem*)((char*)first + 0x5c))
        first->dtor(0);
}