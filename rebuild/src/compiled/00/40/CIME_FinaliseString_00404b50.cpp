struct CIME {
    char pad0[0x8];
    char enabled;     /* +0x8 */
    char pad9[0x47];  /* fill to 0x50 */
    char flag50;      /* +0x50 */
};

extern void __fastcall CIME_sink(bool b);

void __fastcall CIME_FinaliseString(CIME* self, void* /*edx*/, char param)
{
    if (self->enabled) {
        char v = param;
        if (param) {
            if (self->flag50)
                v = 0;
        }
        CIME_sink(v != 0);
    }
}