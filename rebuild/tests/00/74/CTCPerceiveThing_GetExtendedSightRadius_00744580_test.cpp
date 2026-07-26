#include <cstdio>

struct Sub { char pad[0x48]; float f48; float f4c; };
struct Ctx { char pad[0x1c]; void* p1c; };
struct CTCPerceiveThing { char pad[0x34]; Sub* p34; };

int g_a = 0, g_b = 0;
Ctx g_ctx;
float g_mul = 2.0f;

extern "C" Ctx* __fastcall GetCtx(CTCPerceiveThing* self) { (void)self; g_ctx.p1c = (void*)0x1; return &g_ctx; }
extern "C" char __fastcall CheckA(void* obj) { (void)obj; return (char)g_a; }
extern "C" char __fastcall CheckB(void* obj) { (void)obj; return (char)g_b; }

float __fastcall CTCPerceiveThing_GetExtendedSightRadius(CTCPerceiveThing* self)
{
    Ctx* c = GetCtx(self);
    if (CheckA(c->p1c))
        return self->p34->f48;
    c = GetCtx(self);
    if (CheckB(c->p1c))
        return self->p34->f4c;
    Sub* s = self->p34;
    return (s->f4c + s->f48) * g_mul;
}

int main()
{
    Sub s; s.f48 = 3.0f; s.f4c = 5.0f;
    CTCPerceiveThing self; self.p34 = &s;

    g_a = 1; g_b = 0;
    if (CTCPerceiveThing_GetExtendedSightRadius(&self) != 3.0f) { std::printf("FAIL a\n"); return 1; }

    g_a = 0; g_b = 1;
    if (CTCPerceiveThing_GetExtendedSightRadius(&self) != 5.0f) { std::printf("FAIL b\n"); return 1; }

    g_a = 0; g_b = 0;
    if (CTCPerceiveThing_GetExtendedSightRadius(&self) != 16.0f) { std::printf("FAIL c\n"); return 1; }

    std::printf("CTCPerceiveThing_00744580_TEST PASS\n");
    return 0;
}