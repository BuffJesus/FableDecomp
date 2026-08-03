#include <cstdio>
struct ThingRef { unsigned int a; unsigned int b; };
struct CMessageEventFilter { char pad[0x34]; unsigned int uid; unsigned int idx; };
ThingRef * __fastcall IsOriginatedByThing(CMessageEventFilter *self, int edxfill, ThingRef *out)
{
    (void)edxfill;
    out->a = self->uid;
    out->b = self->idx;
    return out;
}
int main()
{
    CMessageEventFilter obj;
    obj.uid = 0xDEADBEEF;
    obj.idx = 0x12345678;
    ThingRef out;
    ThingRef *pout = &out;
    ThingRef *ret = 0;
    CMessageEventFilter *pobj = &obj;
    __asm {
        mov ecx, pobj
        push pout
        call IsOriginatedByThing
        mov ret, eax
    }
    if (out.a == 0xDEADBEEF && out.b == 0x12345678 && ret == pout) {
        printf("OK_0x0052e890\n");
        return 0;
    }
    printf("BAD a=%08x b=%08x ret=%p pout=%p\n", out.a, out.b, (void*)ret, (void*)pout);
    return 1;
}