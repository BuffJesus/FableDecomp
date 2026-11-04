#include "fable_stlport_vector.h"
#include <stdio.h>
#include <string.h>

class C3DMeshInfo;
class CMeshDataBank
{
public:
    void FreeDeviceSpecificResources();
private:
    unsigned char unknown00_[0x380];
    _STL::vector<C3DMeshInfo*> MeshInfos;
};

struct Info
{
    void** vtable;
    int index;
};

static int calls[8];
static int callCount;
static CMeshDataBank* activeBank;
static C3DMeshInfo** activeBegin;
static bool shrinkOnFirst;

void __fastcall freeInfo(Info* info)
{
    calls[callCount++] = info->index;
    if (shrinkOnFirst && callCount == 1)
        *reinterpret_cast<C3DMeshInfo***>(reinterpret_cast<char*>(activeBank) + 0x384) = activeBegin + 1;
}

int main()
{
    void* vtable[2] = {0, reinterpret_cast<void*>(&freeInfo)};
    Info infos[8];
    C3DMeshInfo* elements[8];
    unsigned char storage[0x3a0];
    int cases = 0;
    for (int count = 0; count != 8; ++count) {
        for (int pattern = 0; pattern != 3; ++pattern) {
            memset(storage, 0, sizeof(storage));
            CMeshDataBank* bank = reinterpret_cast<CMeshDataBank*>(storage);
            for (int i = 0; i != count; ++i) {
                infos[i].vtable = vtable;
                infos[i].index = i;
                elements[i] = pattern == 1 && (i & 1) ? 0 :
                    reinterpret_cast<C3DMeshInfo*>(&infos[i]);
            }
            *reinterpret_cast<C3DMeshInfo***>(storage + 0x380) = elements;
            *reinterpret_cast<C3DMeshInfo***>(storage + 0x384) = elements + count;
            activeBank = bank;
            activeBegin = elements;
            shrinkOnFirst = pattern == 2;
            callCount = 0;
            bank->FreeDeviceSpecificResources();
            int expected = 0;
            for (int i = 0; i != count; ++i) {
                if (pattern == 2 && expected == 1)
                    break;
                if (elements[i]) {
                    if (calls[expected] != i) {
                        printf("FAIL order count=%d pattern=%d i=%d\n", count, pattern, i);
                        return 1;
                    }
                    ++expected;
                }
            }
            if (callCount != expected) {
                printf("FAIL count=%d pattern=%d calls=%d expected=%d\n", count, pattern, callCount, expected);
                return 1;
            }
            ++cases;
        }
    }
    printf("MESH_DATA_BANK_FREE_DEVICE_PASS cases=%d\n", cases);
    return 0;
}
