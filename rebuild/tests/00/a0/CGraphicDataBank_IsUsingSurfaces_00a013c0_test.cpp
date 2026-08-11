#include <cstdio>

struct CGraphicDataBank
{
    bool IsUsingSurfaces();
};

bool CGraphicDataBank::IsUsingSurfaces()
{
    return true;
}

int main()
{
    CGraphicDataBank bank;
    bool r = bank.IsUsingSurfaces();
    if (r == true)
    {
        printf("PASS_IsUsingSurfaces\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}