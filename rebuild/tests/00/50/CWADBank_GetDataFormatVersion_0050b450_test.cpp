#include <cstdio>

// Definition of the function under test (identical to source_cpp body).
int GetDataFormatVersion()
{
    return 2;
}

int main()
{
    int v = GetDataFormatVersion();
    if (v != 2) {
        printf("FAIL got %d\n", v);
        return 1;
    }
    printf("OK_0x0050b450 version=%d\n", v);
    return 0;
}