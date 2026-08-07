#include "rebuild_abi.h"
// CTCCarrying::SetAllCarriedThingsAsBypassFilter @ 0x006a2520
struct FableCarry_006a2520
{
    char pad[0x1c];
    unsigned char field_1c;   // +0x1c
    char pad2[3];
    unsigned long field_20;   // +0x20
    void helper();
    void SetAll(unsigned long arg);
};
void FableCarry_006a2520::SetAll(unsigned long arg)
{
    this->field_1c = 1;
    this->field_20 = arg;
    this->helper();
}