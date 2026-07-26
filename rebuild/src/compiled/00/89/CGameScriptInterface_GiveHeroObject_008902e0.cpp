/* CGameScriptInterface::GiveHeroObject @ 008902e0  (VC7.1 / MSVC 2003, x86-32)
 *
 * Byte-exact reconstruction (137 bytes, ret 0xc).
 *
 * Register-allocation / frame notes that make VC7.1 reproduce retail:
 *   - Every engine callee is modeled as a REAL member function (dummy wrapper
 *     struct) so VC7.1 emits __fastcall: 'this' in ecx, all remaining args on
 *     the stack, edx untouched.  Modeling them as free __fastcall(this,edx,...)
 *     forced a spurious 'xor edx,edx' / 'mov edx,arg' before each call.
 *   - CCharString is a SINGLE-POINTER (4-byte) type.  Because the address-taken
 *     temporary is only one dword, VC7.1 reuses the now-dead incoming 'objDefId'
 *     argument slot as its backing store and omits the frame reservation
 *     entirely (no 'sub esp,0x14' / 'add esp,0x14').  A wider struct forces a
 *     dedicated 'sub esp' frame and shifts every [esp+X] offset.
 *
 *   this            -> edi
 *   [this+0x10]     -> object def container (CountObjects(objDefId))
 *   [this+0x14]     -> world  (GetWorldAccessor -> GetHero)
 *   vtable slot 0x1c-> inventory/UI refresh (called twice on this)
 *   ret 0xc         -> 3 incoming dwords: objDefId, quantity, suppressFlag
 */

/* literal C-string operand at abs 0x122d70e (relocation-masked -> extern) */
extern const char GHO_ItemName[];

/* global at ds:0x13d2838 -- gate for the post-give UI refresh */
extern void* GHO_RefreshGate;

/* engine string: single opaque pointer; ctor/dtor manage internals */
struct CCharString {
    void* rep;
};

/* --- engine callees modeled as real (__fastcall) member functions --- */
struct ObjContainer { int  CountObjects(int objDefId); };            /* 0x11d130 */
struct World        { void* GetWorldAccessor(); };                   /* 0xffbb9690 */
struct Accessor     { void* GetHero(); };                            /* 0xffbf7ae0 */
struct Hero         { unsigned char Give(int count, int quantity,
                                         int suppressFlag, CCharString* name); }; /* 0xffe1bf20 */
struct Str          { void ctor(const char* cstr, int len);          /* 0x10e910 */
                      void dtor(); };                                /* 0x10e800 */

void __fastcall CGameScriptInterface_GiveHeroObject(void* thisptr, unsigned long /*edx*/,
                                                    int objDefId, int quantity, int suppressFlag)
{
    void* self = thisptr;                                    /* edi */

    /* mov ecx,[edi+0x10]; push objDefId; call 0x11d130 */
    int count = ((ObjContainer*)(*(void**)((char*)self + 0x10)))->CountObjects(objDefId); /* ebp */
    if (count <= 0)                                          /* test ebp,ebp; jle */
        return;

    /* mov ecx,[edi+0x14]; call 9690 ; mov ecx,eax; call 7ae0 */
    void* accessor = ((World*)(*(void**)((char*)self + 0x14)))->GetWorldAccessor();
    void* hero = ((Accessor*)accessor)->GetHero();          /* esi */
    if (hero == 0)                                           /* test esi,esi; je */
        return;

    if (*(unsigned char*)((char*)hero + 0x91) & 1)          /* test byte[esi+0x91],1; jne */
        return;

    CCharString name;
    ((Str*)&name)->ctor(GHO_ItemName, -1);                  /* push -1; push 0x122d70e */

    /* push &name; push suppress(ebx); push quantity(edx); push count(ebp);
       mov ecx,esi(hero); call 0xffe1bf20 */
    ((Hero*)hero)->Give(count, quantity, suppressFlag, &name);

    ((Str*)&name)->dtor();                                  /* lea ecx,[local]; call 0x10e800 */

    if ((unsigned char)suppressFlag != 0)                   /* test bl,bl; jne */
        return;

    if (GHO_RefreshGate == 0)                               /* mov eax,ds:0x13d2838; test; je */
        return;

    /* mov eax,[edi]; mov ecx,edi; call [eax+0x1c]  (twice) */
    typedef void (__fastcall *GHO_VfnT)(void*);
    ((GHO_VfnT)(*(void***)self)[0x1c / 4])(self);
    ((GHO_VfnT)(*(void***)self)[0x1c / 4])(self);
}