#include <cstdio>
struct CSubField { char data[4]; };
struct CTransferSink {
    void* seen;
    void Xfer(CSubField* p) { seen = p; }
};
struct CSpecialAbilitiesDrainLifeDataDef {
    char pad0[0x28];
    CSubField sub;
    void Transfer(CTransferSink* sink);
};
void CSpecialAbilitiesDrainLifeDataDef::Transfer(CTransferSink* sink)
{
    sink->Xfer(&this->sub);
}
int main() {
    CSpecialAbilitiesDrainLifeDataDef obj;
    CTransferSink sink; sink.seen = 0;
    obj.Transfer(&sink);
    if (sink.seen == (void*)&obj.sub) {
        std::printf("CSpecialAbilitiesDrainLifeDataDef_00452593_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}