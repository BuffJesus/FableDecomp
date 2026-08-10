
struct CTCBoastingPosition {
    unsigned char pad[0x20];
    unsigned char activated;   /* +0x20 */
    long value;                /* +0x24 */
};

char __fastcall IsPositionActivated(CTCBoastingPosition* self, int /*edx*/, long* out)
{
    if (self->activated) {
        *out = self->value;
    }
    return self->activated;
}