#include <cstdio>
struct CAnimFilterBase;
struct CModeBlock { char pad[0x80]; int filter; };
CAnimFilterBase* __fastcall CModeBlock_GetAnimFilter(CModeBlock* self) {
    return (CAnimFilterBase*)((char*)self + 0x80);
}
int main() {
    CModeBlock b;
    void* r = CModeBlock_GetAnimFilter(&b);
    if (r == (void*)((char*)&b + 0x80)) {
        std::printf("CModeBlock_00406880_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}