#include <cstdio>
#include <cctype>

int __cdecl toupper_thunk(int c)
{
    return toupper(c);
}

int main()
{
    int r1 = toupper_thunk('a');
    int r2 = toupper_thunk('z');
    int r3 = toupper_thunk('A');
    int r4 = toupper_thunk('5');
    if (r1 == 'A' && r2 == 'Z' && r3 == 'A' && r4 == '5') {
        printf("TOUPPER_THUNK_OK\n");
    } else {
        printf("TOUPPER_THUNK_BAD %d %d %d %d\n", r1, r2, r3, r4);
    }
    return 0;
}