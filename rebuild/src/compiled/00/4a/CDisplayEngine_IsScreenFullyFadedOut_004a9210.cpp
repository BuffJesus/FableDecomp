
struct CDisplayEngine {
    char pad_bc[0xbc];
    unsigned char fadeActive;   // +0xbc
    char pad_c8[0xc8 - 0xbc - 1];
    unsigned char flagC8;       // +0xc8
    unsigned char flagC9;       // +0xc9
};

int __fastcall IsScreenFullyFadedOut(CDisplayEngine *thisp)
{
    if (thisp->fadeActive) {
        if (!thisp->flagC9) {
            if (!thisp->flagC8) {
                return 1;
            }
        }
    }
    return 0;
}