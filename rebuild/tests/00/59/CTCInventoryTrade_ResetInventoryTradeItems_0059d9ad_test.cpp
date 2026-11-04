#include "fable_thing_components.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static CTCInventoryTrade* owner;
static int order[2],calls,sell,buy;
void CTCInventoryTrade::RemoveAllItemsFromCategory(NInventory::EInventoryCategoryIdentifier category)
{
    if(this!=owner || calls>=2)abort();
    order[calls++]=category;
    if(category==NInventory::INVENTORY_CATEGORY_IDENTIFIER_TRADE_SELL)sell=0;
    else if(category==NInventory::INVENTORY_CATEGORY_IDENTIFIER_TRADE_BUY)buy=0;
    else abort();
}
int main()
{
    unsigned long storage[0x300/4],before[0x300/4];
    owner=reinterpret_cast<CTCInventoryTrade*>(storage+4);
    for(int scenario=0;scenario<4;++scenario) {
        memset(storage,0x31+scenario,sizeof(storage));memcpy(before,storage,sizeof(storage));
        sell=scenario&1?7:0;buy=scenario&2?9:0;calls=0;
        owner->ResetInventoryTradeItems();
        if(calls!=2 || order[0]!=12 || order[1]!=11 || sell || buy || memcmp(storage,before,sizeof(storage)))return 1;
    }
    puts("RESET_INVENTORY_TRADE_OK cases=4");return 0;
}
