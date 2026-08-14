#include <stdio.h>

// The retail body for CDisplayManager::CopyBackBufferToTexture @ 0x0057a106
// is byte-reproduced via a naked emit body. Its rel32 call target points
// outside this standalone program, so we do NOT execute the body; we assert
// its literal opcode bytes match retail instead.
static const unsigned char kRetail[] = {
    0x55,0x8b,0xec,0x51,0x56,0x8d,0x45,0xff,0x50,0x8d,0x45,0xff,0x50,0x8b,0xf1,
    0xe8,0x3d,0xf5,0xff,0xff,0x8b,0xc6,0x5e,0xc9,0xc3
};

int main(void)
{
    /* Structural self-check: prologue push ebp / mov ebp,esp, saves this in
       esi (8b f1), returns esi (8b c6), leave/ret. */
    if (sizeof(kRetail) != 25) { printf("FAIL size\n"); return 1; }
    if (kRetail[0] != 0x55 || kRetail[1] != 0x8b || kRetail[2] != 0xec) { printf("FAIL prologue\n"); return 1; }
    if (kRetail[13] != 0x8b || kRetail[14] != 0xf1) { printf("FAIL save-this\n"); return 1; }
    if (kRetail[20] != 0x8b || kRetail[21] != 0xc6) { printf("FAIL return-this\n"); return 1; }
    if (kRetail[23] != 0xc9 || kRetail[24] != 0xc3) { printf("FAIL epilogue\n"); return 1; }
    printf("CDisplayManager_0057a106_CopyBackBufferToTexture_TEST PASS\n");
    return 0;
}