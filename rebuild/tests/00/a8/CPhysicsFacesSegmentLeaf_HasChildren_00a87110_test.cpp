#include <cstdio>
bool __fastcall CPhysicsFacesSegmentLeaf_HasChildren()
{
    return false;
}
int main()
{
    if (CPhysicsFacesSegmentLeaf_HasChildren() == false) { std::printf("AUTO_TINY_00a87110_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a87110_TEST FAIL\n");
    return 1;
}