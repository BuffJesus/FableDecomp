struct CAIStateGroup_BuyFromSeller {
    char pad[0x8c];
    int result;
    int Helper();
    void BoughtSomething();
};

void CAIStateGroup_BuyFromSeller::BoughtSomething() {
    this->result = this->Helper();
}