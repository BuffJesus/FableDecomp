#include "rebuild_abi.h"
#include <cstdio>
#include <cstring>
struct FableNode_009fd100 { void OnPostParentRelocation(void* value); };
void FableNode_009fd100::OnPostParentRelocation(void* value)
{
    *(void**)((char*)this + 0x26c) = value;
    *(void**)((char*)this + 0x19c) = value;
}
int main() {
    unsigned char buf[0x300];
    std::memset(buf, 0, sizeof(buf));
    void* v = (void*)0x12345678;
    ((FableNode_009fd100*)buf)->OnPostParentRelocation(v);
    if (*(void**)(buf + 0x26c) != v) { std::printf("BAD_A\n"); return 1; }
    if (*(void**)(buf + 0x19c) != v) { std::printf("BAD_B\n"); return 1; }
    std::printf("OPPR_009fd100_OK\n"); return 0;
}