struct CAIStateGroup_BuyFromSeller {
    char pad[0x34];
    int result;
    int ResolvePurchase();
    void BoughtSomething();
};
void CAIStateGroup_BuyFromSeller::BoughtSomething() {
    result = ResolvePurchase();
}