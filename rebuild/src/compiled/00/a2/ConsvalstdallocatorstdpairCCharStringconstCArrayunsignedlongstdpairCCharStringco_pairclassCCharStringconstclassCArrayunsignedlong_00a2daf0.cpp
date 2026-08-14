struct CCharString { void* p; };
struct CArrayUL { void* p; };
struct Pair { CCharString first; CArrayUL second; };

// Allocator/self type. Both pair members are constructed via allocator member ctors:
//  0x99ec30 constructs pair->first (CCharString) ; ecx=self (this), &first pushed
//  0xa2b970 constructs pair->second (CArray)     ; ecx=self+4 (second subobject), &second pushed
struct AllocSecond { void ctor2(CArrayUL* arg); };
struct Alloc {
    char   first_sub[4];
    AllocSecond second_sub;
    void ctorFirst(CCharString* dst);
};

void __fastcall Cons_val(Alloc* self, Pair* dst)
{
    if (self) {
        self->ctorFirst(&dst->first);
        self->second_sub.ctor2(&dst->second);
    }
}