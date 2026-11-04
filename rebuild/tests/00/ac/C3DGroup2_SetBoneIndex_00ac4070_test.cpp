// Behaviour fixture for C3DGroup2::SetBoneIndex (provider C3DGroup2_SetBoneIndex_00ac4070.cpp): the bone index (+0) changes
// and the vertex blend list (+4..+0xC) is untouched; GetBoneIndex reads it back.
#include <stdio.h>
#include <string.h>
#include "fable_3d_group2.h"

int main()
{
    unsigned long raw[4] = { 7, 0x1111, 0x2222, 0x3333 };
    C3DGroup2* group = reinterpret_cast<C3DGroup2*>(raw);
    group->SetBoneIndex(42);
    bool ok = raw[0] == 42 && raw[1] == 0x1111 && raw[2] == 0x2222 && raw[3] == 0x3333 && group->GetBoneIndex() == 42;
    group->SetBoneIndex(-1);
    ok = ok && raw[0] == (unsigned long)-1;
    if (!ok) { printf("GROUP2_SET_BONE_INDEX_FAIL\n"); return 1; }
    printf("GROUP2_SET_BONE_INDEX_OK\n");
    return 0;
}
