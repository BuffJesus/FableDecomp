#include "rebuild_abi.h"
#include <cstdio>
struct FableTrain_006d6810
{
    char pad[0x24];
    long* table;
    void Modify(long index, long delta);
};
int main()
{
    long arr[8] = {0,0,0,0,0,0,0,0};
    FableTrain_006d6810 t; t.table = arr;
    t.Modify(3, 10);
    t.Modify(3, 5);
    if (arr[3] != 15) { std::printf("BAD\n"); return 1; }
    std::printf("MODTRAIN_006d6810_OK\n");
    return 0;
}