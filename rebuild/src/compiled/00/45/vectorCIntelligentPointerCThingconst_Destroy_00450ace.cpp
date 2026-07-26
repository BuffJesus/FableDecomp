#pragma optimize("s",on)
struct Elem {
    virtual void destroy(int flag);
};

void __fastcall vec__Destroy(Elem* first, Elem* last, int alloc) {
    for (; first != last; first = (Elem*)((char*)first + 0x50)) {
        first->destroy(0);
    }
}