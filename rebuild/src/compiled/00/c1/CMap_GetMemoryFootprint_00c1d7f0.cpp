// CMap::GetMemoryFootprint @ 0x00c1d7f0  __fastcall long(CMap*)
struct CMap {
    int   field0;
    int   field4;
    long  field8;   // [esi+8]
    long GetSubMemA();      // 0xc1e9f0
    long GetSubMemB();      // 0xc1e9c0
    long GetMemoryFootprint();
};

long CMap::GetMemoryFootprint()
{
    long total = this->GetSubMemA();
    total += this->GetSubMemB();
    return this->field8 + total;
}