// Retail 0x0059D9AD; Ego_r ResetInventoryTradeItems 0x0060C1A9.
#include "fable_thing_components.h"

void CTCInventoryTrade::ResetInventoryTradeItems()
{
    RemoveAllItemsFromCategory(NInventory::INVENTORY_CATEGORY_IDENTIFIER_TRADE_SELL);
    RemoveAllItemsFromCategory(NInventory::INVENTORY_CATEGORY_IDENTIFIER_TRADE_BUY);
}
