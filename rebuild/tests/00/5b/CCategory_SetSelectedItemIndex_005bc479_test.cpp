#include <cstdio>

struct CCategory {
    char pad[0x20];
    long field20;
    long field24;
    void SetSelectedItemIndex(long index);
};

void CCategory::SetSelectedItemIndex(long index)
{
    this->field20 = index;
    this->field24 = index;
    if (index == -1)
        this->field24 = 0;
}

int main()
{
    CCategory c;
    bool ok = true;

    c.field20 = 999; c.field24 = 999;
    c.SetSelectedItemIndex(5);
    if (c.field20 != 5 || c.field24 != 5) ok = false;

    c.field20 = 999; c.field24 = 999;
    c.SetSelectedItemIndex(-1);
    if (c.field20 != -1 || c.field24 != 0) ok = false;

    c.field20 = 999; c.field24 = 999;
    c.SetSelectedItemIndex(0);
    if (c.field20 != 0 || c.field24 != 0) ok = false;

    if (ok) printf("BEHAVIOR_PASS\n");
    else printf("BEHAVIOR_FAIL\n");
    return 0;
}