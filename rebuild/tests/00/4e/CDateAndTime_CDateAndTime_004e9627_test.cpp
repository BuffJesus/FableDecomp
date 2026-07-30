#include <stdio.h>

struct CDateAndTime {
    float  f00;
    int    pad04[4];
    int    m14;
    int    m18;
    int    m1c;
    int    m20;
    int    m24;
    int    m28;
};
CDateAndTime * __fastcall CDateAndTime(CDateAndTime *this_)
{
    this_->f00 = 0.0f;
    this_->m14 = 0;
    this_->m18 = 0;
    this_->m1c = 0;
    this_->m20 = 0;
    this_->m24 = 0;
    this_->m28 = 0;
    return this_;
}

int main(void)
{
    struct CDateAndTime obj;
    unsigned char *p = (unsigned char *)&obj;
    int i;
    for (i = 0; i < (int)sizeof(struct CDateAndTime); ++i) p[i] = 0xAB;
    obj.pad04[0] = 0x11111111;

    struct CDateAndTime *ret;
    void *self = &obj;
    __asm {
        mov ecx, self
        call CDateAndTime
        mov ret, eax
    }

    int ok = 1;
    if (ret != &obj) ok = 0;
    if (obj.f00 != 0.0f) ok = 0;
    if (obj.m14 != 0) ok = 0;
    if (obj.m18 != 0) ok = 0;
    if (obj.m1c != 0) ok = 0;
    if (obj.m20 != 0) ok = 0;
    if (obj.m24 != 0) ok = 0;
    if (obj.m28 != 0) ok = 0;
    if (obj.pad04[0] != 0x11111111) ok = 0;

    if (ok) { printf("OK_0x004e9627\n"); return 0; }
    printf("BAD f00=%f m14=%d m28=%d pad=%08x\n", obj.f00, obj.m14, obj.m28, obj.pad04[0]);
    return 1;
}