#include <cstdio>

struct CAFile { unsigned char pad[4]; };

bool __fastcall CAFile_IsIOFinished(const CAFile* self)
{
    (void)self;
    return true;
}

int main()
{
    CAFile f;
    if (CAFile_IsIOFinished(&f) == true) {
        std::printf("CAFile_00401650_TEST PASS\n");
        return 0;
    }
    std::printf("CAFile_00401650_TEST FAIL\n");
    return 1;
}