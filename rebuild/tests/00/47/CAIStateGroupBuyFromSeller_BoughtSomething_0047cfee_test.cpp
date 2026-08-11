#include <cstdio>

struct CAIStateGroup_BuyFromSeller {
    char pad[0x8c];
    int result;
    int Helper();
    void BoughtSomething();
};

int CAIStateGroup_BuyFromSeller::Helper() {
    return 0x1234abcd;
}

void CAIStateGroup_BuyFromSeller::BoughtSomething() {
    this->result = this->Helper();
}

int main() {
    CAIStateGroup_BuyFromSeller obj;
    obj.result = 0;
    obj.BoughtSomething();
    if (obj.result == 0x1234abcd) {
        printf("BUYFROMSELLER_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}