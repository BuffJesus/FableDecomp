// Behaviour model for SetAsGameStatePersisted: sets bit2 (mask 0x04) at +0x91 to the bool.
#include <cstdio>

static unsigned char setbit2(unsigned char cur, bool value)
{
    return (unsigned char)((cur & ~0x04u) | ((value ? 1u : 0u) << 2));
}

int main()
{
    bool ok = true;
    // From cleared bit
    if (setbit2(0x00, true)  != 0x04) ok = false;
    if (setbit2(0x00, false) != 0x00) ok = false;
    // Preserve other bits
    if (setbit2(0xFB, true)  != 0xFF) ok = false; // 0xFB = ~0x04
    if (setbit2(0xFF, false) != 0xFB) ok = false;
    // Idempotent set
    if (setbit2(0x04, true)  != 0x04) ok = false;
    if (setbit2(0x04, false) != 0x00) ok = false;
    // Unrelated bits untouched
    if (setbit2(0xAA, true)  != 0xAE) ok = false; // 0xAA|0x04 = 0xAE
    if (setbit2(0xAE, false) != 0xAA) ok = false;
    if (ok) printf("BEHAVIOR_OK\n");
    else    printf("BEHAVIOR_FAIL\n");
    return 0;
}