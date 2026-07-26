struct GSI_MsgKind { unsigned long kind; void* a; void* b; };
struct GSI_MsgArg  { GSI_MsgKind* pInner; void** pB; };

struct MsgFrame { GSI_MsgKind inner; GSI_MsgArg arg; };

/* Message-router object; Post() is __fastcall: 'this' in ecx, &arg pushed. */
struct Router {
    void* Post(GSI_MsgArg* arg);
};

/* engine helpers/globals: relocation-masked, declared extern */
extern void* __fastcall GetA(const void* thisptr);
extern void* __fastcall GetB(const void* thisptr);

/* bool __fastcall CGameScriptInterface::MsgOnHeroCastSpell(void** out)
 * Free __fastcall (this in ecx). One stack arg (out) -> ret 4. */
unsigned char __fastcall Func(void* thisptr, unsigned long edx, void** out)
{
    void* a = GetA(thisptr);
    void* b = GetB(thisptr);

    Router* router = *(Router**)((char*)(*(void**)((char*)thisptr + 0x4)) + 0x60);

    MsgFrame f;
    f.arg.pInner = &f.inner;
    f.inner.b = b;
    f.inner.a = a;
    f.inner.kind = 0x44;
    f.arg.pB = &f.inner.a;

    void* result = router->Post(&f.arg);
    if (result)
    {
        *out = *(void**)((char*)result + 0x3c);
        return 1;
    }
    return 0;
}