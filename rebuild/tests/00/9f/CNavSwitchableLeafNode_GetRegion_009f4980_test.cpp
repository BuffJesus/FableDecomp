#include <cstdio>

struct CNavRegion;

struct CNavSwitchableLeafNode {
    int field_0;
    int field_4;
    CNavRegion* region;   // +8
    char flag;            // +0xc

    bool GetRegion(CNavRegion** out);
};

bool CNavSwitchableLeafNode::GetRegion(CNavRegion** out)
{
    if (flag)
        return false;
    *out = region;
    flag = 1;
    return true;
}

int main()
{
    CNavSwitchableLeafNode n;
    n.field_0 = 0;
    n.field_4 = 0;
    n.region = (CNavRegion*)0xDEADBEEF;
    n.flag = 0;

    CNavRegion* out = 0;
    bool r1 = n.GetRegion(&out);
    // first call: flag was 0 -> returns true, sets out=region, sets flag=1
    if (!r1) { printf("FAIL r1\n"); return 1; }
    if (out != (CNavRegion*)0xDEADBEEF) { printf("FAIL out\n"); return 1; }
    if (n.flag != 1) { printf("FAIL flag\n"); return 1; }

    CNavRegion* out2 = (CNavRegion*)0x12345678;
    bool r2 = n.GetRegion(&out2);
    // second call: flag is 1 -> returns false, out2 unchanged
    if (r2) { printf("FAIL r2\n"); return 1; }
    if (out2 != (CNavRegion*)0x12345678) { printf("FAIL out2\n"); return 1; }

    printf("GETREGION_OK\n");
    return 0;
}