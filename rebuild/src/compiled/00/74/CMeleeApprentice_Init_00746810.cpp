struct S { char pad[0x0C]; unsigned char a; unsigned char b; unsigned char c; };
void __fastcall Init(S* self)
{
    self->a = 1;
    self->b = 0;
    self->c = 0;
}