#include <stdio.h>
#include <string.h>
void* __fastcall Std_Move_Backward(void* first, void* last, void* dest);
int main()
{
    char source[8] = "abcdef"; char out[12]; memset(out,0,sizeof(out));
    if (Std_Move_Backward(source, source, out) != out) return 1;
    if (Std_Move_Backward(source, source + 6, out + 2) != out + 8) return 2;
    if (memcmp(out + 2,"abcdef",6) != 0) return 3;
    char overlap[12] = "abcdef";
    if (Std_Move_Backward(overlap, overlap + 6, overlap + 2) != overlap + 8) return 4;
    if (memcmp(overlap + 2,"abcdef",6) != 0) return 5;
    printf("STD_MOVE_BACKWARD_TEST PASS\n"); return 0;
}
