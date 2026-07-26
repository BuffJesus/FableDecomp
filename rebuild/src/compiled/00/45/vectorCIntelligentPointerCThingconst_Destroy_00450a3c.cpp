#pragma optimize("s",on)
struct Elem { virtual void vf0(int); };

void __fastcall vec_Destroy(Elem* first, Elem* last, int unused) {
    (void)unused;
    while (first != last) {
        first->vf0(0);
        first = (Elem*)((char*)first + 0x50);
    }
}