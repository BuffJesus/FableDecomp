#include "fable_game_text_bank.h"
#include <stdio.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d\n",__LINE__); return 1; } } while(0)
int main() {
    unsigned long bytes[sizeof(CBankFile)/4]; memset(bytes,0,sizeof(bytes));
    CBankFile& bank=*reinterpret_cast<CBankFile*>(bytes);
    const CBankFile& readOnly=bank;
    _STL::pair<unsigned long,unsigned long> entries[65];
    for (unsigned int i=0;i<64;++i) { entries[i].first=(i/2)*0x08000000ul; entries[i].second=i+1; }
    entries[64].first=0xfffffffful; entries[64].second=65;
    bank.SymbolCRCIndices._M_start=entries;
    unsigned int cases=0; unsigned long seed=0x31579bdf;
    const unsigned long edges[]={0,1,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};
    for (unsigned int count=0;count<=65;++count) {
        bank.SymbolCRCIndices._M_finish=entries+count;
        for (unsigned int query=0;query<256;++query) {
            seed=seed*1664525ul+1013904223ul;
            unsigned long key=query<65 ? entries[query].first : (query<71 ? edges[query-65] : seed);
            unsigned int expected=0;
            while (expected<count && entries[expected].first<key) ++expected;
            const _STL::pair<unsigned long,unsigned long>* found=readOnly.SymbolCRCIndices.LowerBound(key);
            CHECK(found==entries+expected);
            unsigned long value=(expected<count && entries[expected].first==key) ? entries[expected].second : 0;
            CHECK(bank.FindIndexBySymbolCRC(key)==value); ++cases;
        }
    }
    printf("BANK_SYMBOL_LOOKUP_PASS cases=%u empty=1 duplicatesFirst=1 unsignedEdges=1\n",cases);
    return 0;
}

#ifdef FABLE_TEST_BANK_SYMBOL_LOWER_BOUND
#include "../../src/compiled/00/9c/CBankFile_FindIndexBySymbolCRC_009cc410.cpp"
#else
#include "../../src/compiled/00/9d/BankSymbolMap_LowerBound_009d20d0.cpp"
#endif
