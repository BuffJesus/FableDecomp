struct S { char pad[4]; short* begin; char gap[4]; short* end; };
int __fastcall Sizeof(S* self)
{
    int n = (int)(self->end - self->begin);
    return n * 2 + 12;
}