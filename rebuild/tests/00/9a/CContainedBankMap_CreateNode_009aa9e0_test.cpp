#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"

typedef NBankFileManager::CContainedBankMap Map;
static int allocations;
static int copies;

extern "C" void* __cdecl FixtureContainedMalloc(size_t size)
{
    if (size != sizeof(Map::CNode)) abort();
    ++allocations;
    return malloc(size);
}

void* __cdecl operator new(size_t) {fputs("CONTAINED_BANK_WRONG_ALLOCATOR\n",stderr);exit(80);return 0;}

CCharString::CCharString(const CCharString& other)
{
    ++copies;
    memcpy(this, &other, sizeof(*this));
}

int main()
{
    fable_u32 raw[6] = {0x12345678, 11, 22, 33, 44, 55};
    unsigned char mapBytes[sizeof(Map)] = {0};
    Map& map = *reinterpret_cast<Map*>(mapBytes);
    const Map::value_type& value = *reinterpret_cast<const Map::value_type*>(raw);
    Map::CNode* node = map.CreateNode(value);
    if (allocations != 1 || copies != 1 || node == 0) return 1;
    if (memcmp(&node->value_, raw, sizeof(raw)) != 0) return 2;
    raw[1] = 99;
    if (node->value_.second.dataVersion_ != 11) return 3;
    free(node);
    puts("CONTAINED_BANK_CREATE_NODE_PASS");
    return 0;
}
