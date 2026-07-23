#include <stdio.h>
extern "C" bool CanBeHitByMeleeStrike(void *self) {
    unsigned char *base = (unsigned char *)self;
    unsigned char *shot = *(unsigned char **)(base + 0x7a0);
    return shot[76] != 0;
}
int main() {
    unsigned char obj[0x7a4];
    unsigned char shot[0x60];
    int i;
    for (i = 0; i < (int)sizeof(obj); ++i) obj[i] = 0;
    for (i = 0; i < (int)sizeof(shot); ++i) shot[i] = 0;
    *(unsigned char **)(obj + 0x7a0) = shot;
    if (CanBeHitByMeleeStrike(obj)) { printf("FAIL initial true\n"); return 1; }
    shot[76] = 1;
    if (!CanBeHitByMeleeStrike(obj)) { printf("FAIL set false\n"); return 1; }
    printf("PASS_00445bb0\n");
    return 0;
}