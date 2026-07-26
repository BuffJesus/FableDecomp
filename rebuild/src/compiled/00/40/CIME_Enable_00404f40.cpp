struct CIME {
    unsigned char _pad_0x0[0x8];
    char enabled;
    unsigned char _pad_0x9[0x47];
    char flag50;
};

extern void __fastcall CIME_cb_a(char b);
extern void __fastcall CIME_cb_b(char b);

void __fastcall CIME_Enable(CIME* self, int edx, char enable)
{
    if (self->enabled) {
        CIME_cb_a(0);
        CIME_cb_b(enable != 0);
    }
}