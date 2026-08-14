// Byte-exact reconstruction of CAIStateGroup_BuyFromSeller::ShouldBuyFromSeller @ 0x00919a10
// __fastcall bool f(Self* this)

struct CAIStateGroup_BuyFromSeller {
    char pad[0x14];
    int  field14; // +0x14
};

// call target at 0x49d870: no-arg getter returning int in eax
extern int __fastcall GetCurrentTick_49d870(void);

// globals
extern int g_div_1375550;   // [0x1375550] divisor
extern int g_cmp_138c694;   // [0x138c694] compare value

bool __fastcall ShouldBuyFromSeller(CAIStateGroup_BuyFromSeller* self)
{
    int elapsed = GetCurrentTick_49d870() - self->field14;
    int quot = elapsed / g_div_1375550;
    if ((unsigned int)quot > (unsigned int)g_cmp_138c694)
        return true;
    return false;
}