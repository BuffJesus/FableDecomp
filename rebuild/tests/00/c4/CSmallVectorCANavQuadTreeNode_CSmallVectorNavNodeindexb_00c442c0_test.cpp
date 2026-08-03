#include <cstdio>

struct CSmallVector_NavNode {
    void** data;

    void** index_b(unsigned long i) {
        return (void**)data[i];
    }
};

int main() {
    void* arr[4];
    void* marker0 = (void*)0x11111111;
    void* marker1 = (void*)0x22222222;
    void* marker2 = (void*)0x33333333;
    void* marker3 = (void*)0x44444444;
    arr[0] = marker0;
    arr[1] = marker1;
    arr[2] = marker2;
    arr[3] = marker3;

    CSmallVector_NavNode vec;
    vec.data = arr;

    void** r0 = vec.index_b(0);
    void** r1 = vec.index_b(1);
    void** r2 = vec.index_b(2);
    void** r3 = vec.index_b(3);

    if (r0 == (void**)marker0 && r1 == (void**)marker1 && r2 == (void**)marker2 && r3 == (void**)marker3) {
        printf("INDEX_B_OK\n");
    } else {
        printf("INDEX_B_FAIL\n");
    }
    return 0;
}