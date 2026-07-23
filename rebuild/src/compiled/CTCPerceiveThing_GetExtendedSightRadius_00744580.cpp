struct Sub { char pad[0x48]; float f48; float f4c; };
struct Ctx { char pad[0x1c]; void* p1c; };
struct CTCPerceiveThing { char pad[0x34]; Sub* p34; };

extern "C" Ctx* __fastcall GetCtx(CTCPerceiveThing* self);
extern "C" char __fastcall CheckA(void* obj);
extern "C" char __fastcall CheckB(void* obj);
extern float g_mul;

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