#pragma optimize("s", on)
struct T { virtual void vdtor(int flags); };
void __fastcall Vec_Destroy(T* first, T* last, void* al)
{
    (void)al;
    for (; first != last; first = (T*)((char*)first + 0x48))
        first->vdtor(0);
}