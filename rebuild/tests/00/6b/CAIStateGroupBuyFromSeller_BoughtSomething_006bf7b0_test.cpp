#include <stdio.h>
struct CAIStateGroup_BuyFromSeller {
    char pad[0x34];
    int result;
    int ResolvePurchase();
    void BoughtSomething();
};
int CAIStateGroup_BuyFromSeller::ResolvePurchase() { return 0x12345678; }
void CAIStateGroup_BuyFromSeller::BoughtSomething() { result = ResolvePurchase(); }
int main() {
    CAIStateGroup_BuyFromSeller state;
    state.result = 0;
    state.BoughtSomething();
    if (state.result != 0x12345678) return 1;
    printf("BOUGHT_SOMETHING PASS\n");
    return 0;
}