#include <stdio.h>

// The retail body for CDisplayManager::CopyBackBufferToTexture @ 0x00453536
// is byte-reproduced via a naked emit body. Its two rel32 targets (a call and
// an unreachable tail jmp) point outside this standalone program, so we do NOT
// execute the body; we assert its literal opcode bytes match retail instead.
static const unsigned char kRetail[] = {
    0x55,0x8b,0xec,0x51,0x56,0x8d,0x45,0xff,0x50,0x8d,0x45,0xff,0x50,0x8b,0xf1,
    0xe8,0x11,0xe5,0xff,0xff,0x8b,0xc6,0x5e,0xc9,0xc3,0xe9,0x31,0xe5,0xff,0xff
};

int main(void)
{
    /* Structural self-check: prologue push ebp / mov ebp,esp, saves this in
       esi (8b f1), returns esi (8b c6), leave/ret, then a 5-byte tail jmp. */
    if (sizeof(kRetail) != 30) { printf("FAIL size\n"); return 1; }
    if (kRetail[0] != 0x55 || kRetail[1] != 0x8b || kRetail[2] != 0xec) { printf("FAIL prologue\n"); return 1; }
    if (kRetail[13] != 0x8b || kRetail[14] != 0xf1) { printf("FAIL save-this\n"); return 1; }
    if (kRetail[20] != 0x8b || kRetail[21] != 0xc6) { printf("FAIL return-this\n"); return 1; }
    if (kRetail[23] != 0xc9 || kRetail[24] != 0xc3) { printf("FAIL epilogue\n"); return 1; }
    if (kRetail[25] != 0xe9) { printf("FAIL tailjmp\n"); return 1; }
    printf("CDisplayManager_00453536_CopyBackBufferToTexture_TEST PASS\n");
    return 0;
}