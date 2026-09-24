#include "../../include/fable_ui_sprite_visibility.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc != 2) return 2;
    FILE* input = fopen(argv[1], "r");
    if (!input) return 3;
    unsigned pending, alpha, xbits, ybits;
    while (fscanf(input, "%u %u %u %u", &pending, &alpha, &xbits, &ybits) == 4)
    {
        float x, y;
        memcpy(&x, &xbits, sizeof(x));
        memcpy(&y, &ybits, sizeof(y));
        fable_u8 flag = static_cast<fable_u8>(pending);
        const FableUiSpriteDisposition decision = FableUiSpriteVisibility(
            flag, static_cast<fable_u8>(alpha), x, y);
        printf("%u %u\n", static_cast<unsigned>(decision), static_cast<unsigned>(flag));
    }
    fclose(input);
    return 0;
}
