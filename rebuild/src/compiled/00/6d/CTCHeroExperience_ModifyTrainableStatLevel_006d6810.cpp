#include "rebuild_abi.h"
// CTCHeroExperience::ModifyTrainableStatLevel @ 0x006d6810
struct FableTrain_006d6810
{
    char pad[0x24];
    long* table;    // +0x24
    void Modify(long index, long delta);
};
void FableTrain_006d6810::Modify(long index, long delta)
{
    this->table[index] += delta;
}