#include <stdio.h>

#include "rebuild_abi.h"

// Byte-identical retail function:
//   MemCmp_Unsigned16 @ 0x00403C60
//
// Stage 0 deliberately links a small proven function. It establishes the
// executable/toolchain boundary without pretending that the retail boot chain
// or game loop is reconstructed yet.
int FABLE_CDECL MemCmp_Unsigned16(
    const unsigned short* lhs,
    const unsigned short* rhs,
    unsigned long count);

static int VerifyLinkedRetailCode()
{
    const unsigned short expected[] = { 0x0001, 0x0010, 0x0100, 0x1000 };
    unsigned short actual[] = { 0x0001, 0x0010, 0x0100, 0x1000 };

    if (MemCmp_Unsigned16(expected, actual, 4) != 0)
        return 1;

    actual[2] = 0x0200;
    if (MemCmp_Unsigned16(expected, actual, 4) != -1)
        return 2;

    actual[2] = 0x0001;
    if (MemCmp_Unsigned16(expected, actual, 4) != 1)
        return 3;

    return 0;
}

int main()
{
    printf("FableTLC reconstruction bootstrap\n");
    printf("  milestone : Stage 0 - original-compiler executable integration\n");
    printf("  target    : Win32 x86\n");
    printf("  compiler  : Microsoft Visual C++ 7.1\n");
    printf("  retail    : MemCmp_Unsigned16 @ 0x00403C60 (byte MATCH)\n");

    const int verificationResult = VerifyLinkedRetailCode();
    if (verificationResult != 0)
    {
        printf("FABLETLC_BOOTSTRAP_STAGE0 FAIL code=%d\n", verificationResult);
        return verificationResult;
    }

    printf("FABLETLC_BOOTSTRAP_STAGE0 PASS\n");
    return 0;
}
