#include <stdio.h>
struct Item{long ItemType;long NumberOfItems;unsigned char data[12];
 bool IsSameItemTypeAsDefIndex(long)const;};
bool Item::IsSameItemTypeAsDefIndex(long index)const{return NumberOfItems!=0 && ItemType==index;}
int main(){Item item;item.ItemType=42;item.NumberOfItems=0;
 if(item.IsSameItemTypeAsDefIndex(42))return 1;item.NumberOfItems=2;
 if(!item.IsSameItemTypeAsDefIndex(42) || item.IsSameItemTypeAsDefIndex(41))return 2;
 printf("ITEM_TYPE_DEF_INDEX PASS\n");return 0;}