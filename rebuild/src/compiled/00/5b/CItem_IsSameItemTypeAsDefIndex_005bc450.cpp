#pragma optimize("s",on)
#include "engine/CItem.h"

struct CItem_TypeMethods : CItem {
    bool IsSameItemTypeAsDefIndex(long definitionIndex) const;
};

bool CItem_TypeMethods::IsSameItemTypeAsDefIndex(long definitionIndex) const {
    if (NumberOfItems == 0) return false;
    return definitionIndex == ItemType;
}