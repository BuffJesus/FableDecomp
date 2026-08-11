#include <cstdio>

struct CNavNavigableLeafNode {
    virtual void v0() {}
    virtual void v1() {}
    virtual void v2() {}
    virtual void v3() {}
    virtual void v4() {}
    virtual int  v5() { return retval; }
    bool RemoveRegionZero();
    int retval;
};

bool CNavNavigableLeafNode::RemoveRegionZero()
{
    return this->v5() == 0;
}

int main()
{
    CNavNavigableLeafNode a;
    a.retval = 0;
    bool r0 = a.RemoveRegionZero();

    CNavNavigableLeafNode b;
    b.retval = 7;
    bool r1 = b.RemoveRegionZero();

    CNavNavigableLeafNode c;
    c.retval = -3;
    bool r2 = c.RemoveRegionZero();

    if (r0 == true && r1 == false && r2 == false) {
        printf("REMOVEREGIONZERO_OK\n");
        return 0;
    }
    printf("FAIL r0=%d r1=%d r2=%d\n", (int)r0, (int)r1, (int)r2);
    return 1;
}