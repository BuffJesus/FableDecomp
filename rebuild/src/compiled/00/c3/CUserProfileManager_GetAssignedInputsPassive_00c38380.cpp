#pragma optimize("s",on)
struct Pair { unsigned int a; unsigned int b; };

bool __fastcall GetAssignedInputsPassive(Pair* self, Pair* other)
{
    if (self->b == other->b)
        return self->a < other->a;
    return self->b < other->b;
}