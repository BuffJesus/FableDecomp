#include "fable_system_manager.h"
#include <stdio.h>
#include <string.h>

class CMeshDataBank
{
public:
    void FreeDeviceSpecificResources();
};

static CMeshDataBank* called[8];
static int callCount;
void CMeshDataBank::FreeDeviceSpecificResources()
{
    called[callCount++] = this;
}

struct Node
{
    Node* next;
    Node* prev;
    unsigned long handle;
    CMeshDataBank* bank;
    void* info;
};

int main()
{
    unsigned char managerStorage[0x40];
    unsigned char bankStorage[8][4];
    Node sentinel;
    Node nodes[8];
    int cases = 0;
    for (int count = 0; count != 8; ++count) {
        for (int offset = 0; offset != 2; ++offset) {
            memset(managerStorage, 0, sizeof(managerStorage));
            CMeshBankManager* manager = reinterpret_cast<CMeshBankManager*>(managerStorage + offset * 4);
            *reinterpret_cast<Node**>(reinterpret_cast<char*>(manager) + 8) = &sentinel;
            sentinel.next = count ? &nodes[0] : &sentinel;
            sentinel.prev = count ? &nodes[count - 1] : &sentinel;
            for (int n = 0; n != count; ++n) {
                nodes[n].next = n + 1 < count ? &nodes[n + 1] : &sentinel;
                nodes[n].prev = n ? &nodes[n - 1] : &sentinel;
                nodes[n].bank = reinterpret_cast<CMeshDataBank*>(bankStorage[n]);
            }
            callCount = 0;
            manager->CMeshBankManager::OnPreDeviceReset();
            if (callCount != count) {
                printf("FAIL count=%d offset=%d calls=%d\n", count, offset, callCount);
                return 1;
            }
            for (int n = 0; n != count; ++n) {
                if (called[n] != nodes[n].bank) {
                    printf("FAIL order count=%d offset=%d n=%d\n", count, offset, n);
                    return 1;
                }
            }
            ++cases;
        }
    }
    printf("MESH_BANK_PRE_RESET_PASS cases=%d\n", cases);
    return 0;
}
