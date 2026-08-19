// _Uninit_copy 12B-stride element (3 dwords; 3rd is refcounted). base /O2 /Oy. family len56 x7 (00448280).
struct Elem { void* a; void* b; long* c; };
struct Tag {};
Elem* __fastcall Uninit_copy(Elem* first, Elem* last, Elem* dest, Tag t) {
    while (first != last) {
        if (dest) { dest->a=first->a; dest->b=first->b; long* p=first->c; dest->c=p; if(p) ++*p; }
        ++first; ++dest;
    }
    return dest;
}