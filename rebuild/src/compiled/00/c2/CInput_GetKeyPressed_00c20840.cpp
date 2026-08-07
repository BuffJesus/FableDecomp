struct S { char pad[0xAC]; unsigned short k; };
bool __fastcall GetKeyPressed(S* self)
{
    return self->k != 0;
}