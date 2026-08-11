#include <stdio.h>

struct Inner {
    int magic;
    int CheckValid();
};

struct CAIStateGroup_MinionKidGroupTag {
    char pad[0x18];
    Inner* inner;        // +0x18
    int IsStillValid();
};

static int g_ret = 1;
int Inner::CheckValid() { return g_ret; }

int CAIStateGroup_MinionKidGroupTag::IsStillValid()
{
    Inner* p = this->inner;
    if (p != 0 && p->CheckValid())
        return 1;
    return 0;
}

int main()
{
    CAIStateGroup_MinionKidGroupTag obj;
    Inner inObj; inObj.magic = 7;

    obj.inner = 0;
    if (obj.IsStillValid()) { printf("FAIL null\n"); return 1; }

    obj.inner = &inObj; g_ret = 1;
    if (!obj.IsStillValid()) { printf("FAIL true\n"); return 1; }

    g_ret = 0;
    if (obj.IsStillValid()) { printf("FAIL false\n"); return 1; }

    printf("ISVALID_OK\n");
    return 0;
}