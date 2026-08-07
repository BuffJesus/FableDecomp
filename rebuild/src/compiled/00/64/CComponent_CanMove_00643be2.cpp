#pragma optimize("s",on)
struct V2 { int a; int b; };
V2* __fastcall CanMove(V2* self, V2* src)
{
    self->a = src->a;
    self->b = src->b;
    return self;
}