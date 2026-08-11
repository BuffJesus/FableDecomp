// GFPredicateAnd<...>::CPredicate_And forwarder at 0x006b8350
// Retail:
//   mov eax,[esp+8]      ; p2
//   push esi
//   mov esi,[esp+8]      ; p1
//   push eax             ; p2
//   push esi             ; p1
//   add ecx,0x90         ; &this->inner
//   call 0x880f60        ; inner.ctor(p1, p2)
//   mov eax,esi          ; return p1
//   ret 8

struct CIsThingAlive;

struct Inner {
    // subobject at this+0x90 whose method takes (p1, p2)
    char pad[4];
    CIsThingAlive* build(CIsThingAlive* a, CIsThingAlive* b);
};

struct Outer {
    char pad[0x90];
    Inner inner;
    CIsThingAlive* CPredicate_And(CIsThingAlive* p1, CIsThingAlive* p2);
};

CIsThingAlive* Outer::CPredicate_And(CIsThingAlive* p1, CIsThingAlive* p2)
{
    inner.build(p1, p2);
    return p1;
}