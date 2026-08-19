// _Uninit_copy<pair<u64-ish 8B stride, refcounted 2nd dword>>. __fastcall first=ecx,last=edx,
// dest+tag on stack (ret 8). base /O2 /Oy (no pragma). family len50 x31 (template 004477c0).
struct Elem { void* a; long* b; };
struct Tag {};
Elem* __fastcall Uninit_copy(Elem* first, Elem* last, Elem* dest, Tag t) {
    while (first != last) {
        if (dest) { dest->a=first->a; long* p=first->b; dest->b=p; if(p) ++*p; }
        ++first; ++dest;
    }
    return dest;
}