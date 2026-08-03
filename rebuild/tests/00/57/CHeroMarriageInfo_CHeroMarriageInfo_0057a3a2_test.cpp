#include <stdio.h>
struct MI { int a; int b; int c; int d; int e; char f; };
struct MI* __fastcall CHeroMarriageInfo(struct MI* self, int, const struct MI* src)
{
    self->a = src->a;
    self->b = src->b;
    self->c = src->c;
    self->d = src->d;
    self->e = src->e;
    self->f = src->f;
    return self;
}
int main()
{
    struct MI src; src.a=11; src.b=22; src.c=33; src.d=44; src.e=55; src.f=(char)0x7A;
    struct MI dst; dst.a=dst.b=dst.c=dst.d=dst.e=0; dst.f=0;
    struct MI* ret = 0;
    void* pself = &dst;
    const void* psrc = &src;
    __asm {
        push psrc      ; stack arg src (esp+4 after call)
        mov  ecx, pself ; this
        call CHeroMarriageInfo
        mov  ret, eax
    }
    if (ret != &dst) { printf("BAD ret\n"); return 1; }
    if (dst.a==11 && dst.b==22 && dst.c==33 && dst.d==44 && dst.e==55 && dst.f==(char)0x7A) {
        printf("OK_0x0057a3a2\n");
        return 0;
    }
    printf("BAD fields %d %d %d %d %d %d\n", dst.a,dst.b,dst.c,dst.d,dst.e,(int)dst.f);
    return 1;
}